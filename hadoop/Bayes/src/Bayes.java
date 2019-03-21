

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.IdentityHashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class Bayes
{
	boolean m_isLoadModel;
	List<String> m_className = new ArrayList<String>();
	List<List<Integer>> m_ClassList = new ArrayList<List<Integer>>();
	Map<Integer, List<Float>> m_ClassAttrSet = new HashMap<Integer, List<Float>>();
	List<Map<String, Float>> m_Model = new ArrayList<Map<String, Float>>();
	List<Model> m_ModelList = new ArrayList<Model>();
	Map<String, Model> m_classNM = new HashMap<String, Model>();
	List<Integer> m_current_file_num = new ArrayList<Integer>();
	
	int BYSetClassList(List<List<Integer>> vclass)
	{
		m_ClassList.addAll(vclass);
		return 0;
	}
	
	void uniqueList(List<Float> list)
	{
		int index = 0;
		while(index +1 <= list.size())
		{
			Float[] la = (Float[]) list.toArray();
			index = 0;
			while(index +1 <= list.size())
			{
				if(la[index].floatValue() == la[index+1].floatValue())
				{
					list.remove(index);
					break;
				}
				++index;
			}
		}
	}
	
	void MapToMultiMap(Map<Float, Float> src, IdentityHashMap<Float, Float> des)
	{
		Iterator<Float> iter = src.keySet().iterator();
		while(iter.hasNext())
		{
			Float k = iter.next();
			des.put(src.get(k), k);
		}
	}	
	
	int BYSelectAttributeFromClassFile(List<Map<Float,Float>> tfidf, 
									   List<List<Integer>> vclass, 
									   List<List<Float>> classattribute)
	{
		Integer Kn = 0;
		Integer index = 0;
	
		Kn = vclass.size();
		
		Iterator<List<Integer>> pvclass = vclass.iterator();
		while(pvclass.hasNext())
		{
			Map<Float, Float> ClassAttr = new HashMap<Float, Float>();
			List<Float> oneClassAttr = new ArrayList<Float>();
	      Integer file_num=0;
	      
	      
	      List<Integer> ovclass = pvclass.next();
			Iterator<Integer> fileid = ovclass.iterator();
			while(fileid.hasNext())
			{
				Integer tid = fileid.next();
				
		          if(tid.intValue() >= tfidf.size())
		            {
		                //++fileid;
		                //++file_num;
		                break;
		            }
					
					Map<Float, Float> pFile = tfidf.get(tid);
					
					IdentityHashMap<Float, Float> mMap = new IdentityHashMap<Float, Float>();
		
					MapToMultiMap(pFile, mMap);
					TFIDF.ReverseSelectAttrFromIdentityHashMap(mMap, oneClassAttr);
		
		         ++file_num;
			}
			
			
			
		      m_current_file_num.add(index, file_num);
		  	
		      oneClassAttr.sort(new Comparator<Float>(){

				@Override
				public int compare(Float o1, Float o2) {
					// TODO Auto-generated method stub
					if(o1.floatValue() > o2.floatValue())
		      			return 1;
					return 0;
				}});
		      
				uniqueList(oneClassAttr);
				classattribute.add(oneClassAttr);
		
				m_ClassList.add(index,ovclass);
				++index;
		}
	
		return 0;
	}
	
	
	int BYSetClassAttribute(Integer ClassId, List<Float> classatrribute)
	{
		if(m_ClassAttrSet.get(ClassId) != null)
			return -1;
	
		m_ClassAttrSet.put(ClassId, classatrribute);
		return 0;
	}
	
	
	int BYCreateWordProbabilityInClass(List<List<Float>> content, Integer ClassId, Map<String, Float> wordPro)
	{
	
		Float ClassSumWords = new Float(0);
		Float Words = new Float(0);
		
		List<Integer> classidset = m_ClassList.get(ClassId);
		if(classidset == null)
			return -1;
		
		Iterator<Integer> csSumWords = classidset.iterator();
		while(csSumWords.hasNext())
		{
			Integer tI = csSumWords.next();
			if(tI.intValue() >= content.size())
			{
				break;
			}
			ClassSumWords += content.get(tI).size();
		}
	
		List<Float> pClassAttr = m_ClassAttrSet.get(ClassId);
		if (pClassAttr == null)
		{
			return -1;
		}
		
		Integer statistics = 0;
		for(Float pAttr : pClassAttr)
		{
			Words = new Float(0);
			
			for(Integer id : classidset)
			{
	            if(id.intValue() >= content.size())
	            {
	                break;
	            }
	
				Iterator<Float> pWord = content.get(id).iterator();
				while(pWord.hasNext())
				{
					if (pWord.next() == pAttr)
					{
						++Words;
					}
				}
			}
	
			Float p = Words / ClassSumWords;
	
			StringBuffer word = new StringBuffer();
			//FindWordsById(IdToWords, *pAttr, word);
			//GetWordById(pAttr, word);
	
			//of <<word.c_str()<<":"<<p<<endl;
			wordPro.put(word.toString(), p);

			++statistics;
		}

		return 0;
	}
	
	
	Integer CalculateSimilaritryRange(List<List<String>> DatawordMatrix, 
								  Integer ClassId, 
								  Map<String, Float> wordPro,
								  Float MinSimilarity,
								  Float MaxSimilarity
								  )
	{		
		List<Integer> classidset = m_ClassList.get(ClassId);
		if(classidset == null)
			return -1;
		
		MinSimilarity = new Float(1);
		MaxSimilarity = new Float(0);
		Iterator<Integer> pFile = classidset.iterator();
		while(pFile.hasNext())
		{
			Integer tFile = pFile.next();
			Float Similarity = new Float(0);
			
			Map<String, Float> ctWordsList = new HashMap<String, Float>();
	        if(tFile.intValue() >= DatawordMatrix.size())
	        {
	            break;
	        }
	
			List<String> dm = DatawordMatrix.get(tFile.intValue());
			CalculateWordProInFile(dm, ctWordsList);
			CalculateFileSimilarityInModel(wordPro, ctWordsList, Similarity);
	
			if (Similarity.floatValue() <MinSimilarity.floatValue())
			{
				MinSimilarity = Similarity;
			}
	
			if (Similarity.floatValue() > MaxSimilarity.floatValue())
			{
				MaxSimilarity = Similarity;
			}
		}
	
		return 0;
	}
	
	Integer BYCreateClassModel(List<List<Float>> content, 
							List<List<String>> DatawordMatrix, 
							Integer ClassId, 
							String ModelFile, Float threshold)
	{
		Map<String, Float> wordPro = new HashMap<String, Float>();
		BYCreateWordProbabilityInClass(content, ClassId, wordPro);
	
		Float MinSimilarity = new Float(0);
		Float MaxSimilarity = new Float(0);
		CalculateSimilaritryRange(DatawordMatrix, 
			ClassId, 
			wordPro,
			MinSimilarity,
			MaxSimilarity
			);
	
		Model pModel = new FileModel(ModelFile);
	
	    if(m_className.size() == 0)
	    {
		    String mn = "test";
		    pModel.setmodulename(mn);
	    }
	    else
	    {
	       pModel.setmodulename(m_className.get(ClassId));
	    }
		Float[] thresholdrange = new Float[2];
		thresholdrange[0] = MinSimilarity;
		thresholdrange[1] = MaxSimilarity;
		pModel.setthresholdrange(thresholdrange);
	
	    if(threshold> 0)
		    pModel.setthreshold(threshold);
	    else
		    pModel.setthreshold(MinSimilarity);
	
		pModel.setwordpro(wordPro);
		pModel.m_filenumber = m_current_file_num.get(ClassId);
	
		pModel.savemodule();
	
		return 0;
	}
	
	int BYLoadClassModel(String ModelDir) throws IllegalArgumentException, IOException
	{
		List<String> FileList = new ArrayList<String>();
	
		//if (!Dic->DicOk())
		//{
		//	return -2;
		//}
	
		if (m_isLoadModel == true)
		{
			return -1;
		}
	
		oper_hdfs oh = new oper_hdfs();
		oh.initRemoteHdfs();
		oh.traversal_diretory(ModelDir, FileList);
		
		if(FileList.size() <= 1)
		{
			return -3;
		}
	
		int ok_n = 0;
		for(String pModelFile : FileList)
		{
			FileModel pModel = new FileModel(pModelFile);
			pModel.loadmodule();
			m_ModelList.add(pModel);
			m_classNM.put(pModel.getmodulename(), pModel);
			++ok_n;
			
			if(ok_n> 1)
				m_isLoadModel = true;
		}
	
		return 0;
	}
	
	int SetThreshold4(Float threshold)
	{
		//g_threshold = threshold;
		return 0;
	}
	
	int CalculateWordProInFile(List<String> Words, Map<String, Float> ctWordsList)
	{
		Float ctCount = new Float(Words.size());
		
		Iterator<String> pOneWord = Words.iterator();
		while(pOneWord.hasNext())
		{
			String tWord = pOneWord.next();
			Float pctList = ctWordsList.get(tWord);
			if(pctList == null)
				ctWordsList.put(tWord, new Float(1/ctCount));
			else
				ctWordsList.put(tWord, new Float(pctList + 1/ctCount));
		}
		
		return 0;
	}
	
	Integer CalculateFileProInModel(Integer ModelId, List<String> Words, Float Probability, List<String> KeyWords)
	{
		Float lProbability = new Float(0);
		
		Map<String, Float> pModel = m_Model.get(ModelId);
		
		Iterator<String> pWords = Words.iterator();
		while(pWords.hasNext())
		{
			String tWord = pWords.next();
			
			Float pModelWord = pModel.get(tWord);
			if (pModelWord != null)
			{
				KeyWords.add(tWord);
				lProbability += pModelWord;
			}
		}
	
		//Probability = Probability / Words.size();
		
		Probability = lProbability;
		return 0;
	}
	
	Integer CalculateFileSimilarityInModel(Map<String, Float> Model, Map<String, Float> ctWordsList, Float Similarity)
	{
		Float lSimilarity = new Float(0);
		
		Iterator<String> pWords = ctWordsList.keySet().iterator();
		while(pWords.hasNext())
		{
			String tWord = pWords.next();
			
			Float pModelWord = Model.get(tWord);
			if(pModelWord != null)
			{
				lSimilarity += ctWordsList.get(tWord);
			}
		}
	
		//Probability = Probability / Words.size();
	
		Similarity = lSimilarity;
		return 0;
	}
	
	boolean IsAttractiveContent(String Content, String uid, String level, String ClassName, Float oSimilarity, List<String> KeyWords)
	{
		boolean bRtn = false;
		List<List<Float>> ProbabilityList = new ArrayList<List<Float>>();
	
		if (!m_isLoadModel)
		{
			return false;
		}
	
		float Probability = 0;
		float similarity = 0;
		List<String> Words = new ArrayList<String>();
		//Dic->SplitWord(Content, Words);
	   if(Words.size() <10)
	      return false;
	
		Map<String, Float> ctWordsList = new HashMap<String, Float>();
		CalculateWordProInFile(Words, ctWordsList);
		
		//List<Map<String, Float>>::iterator pModel = g_Model.begin();
		Iterator<Model> pModel = m_ModelList.iterator();
		while(pModel.hasNext())
		{
			Probability = 0;
			similarity = 0;
			Model tModel = pModel.next();
			
			Map<String, Float> pModePro = tModel.getwordpro();
			Iterator<String> pctList = ctWordsList.keySet().iterator();
			while(pctList.hasNext())
			{
				String tWord = pctList.next();
				Float pModelWord = pModePro.get(tWord);
				if(pModelWord != null)
				{
					KeyWords.add(tWord);
					Probability += pModelWord;
					similarity += ctWordsList.get(tWord);
				}
			}
	
			//Probability = Probability / Words.size();
	
	      Probability = tModel.m_filenumber;
			List<Float> oneResult = new ArrayList<Float>();
			oneResult.add(Probability);
			oneResult.add(similarity);
			ProbabilityList.add(oneResult);
		}
		
		int MaxProb = 0, MaxIndex=0, i=0;
		for(List<Float> pIter : ProbabilityList)
		{
			if(MaxProb < pIter.get(0))
			{
				MaxProb = pIter.get(0).intValue();
				MaxIndex = i;
			}
			
			++i;
		}
		
		List<Float> oneProb = ProbabilityList.get(MaxIndex);
		if(oneProb.get(1) >= m_ModelList.get(MaxIndex).getthreshold())
		{
			oSimilarity = oneProb.get(1);
			ClassName = m_ModelList.get(MaxIndex).getmodulename();
			uid = m_ModelList.get(MaxIndex).getuid();
			level = m_ModelList.get(MaxIndex).getlevel();
			bRtn = true;
		}
	
		return bRtn;
	}
	
	boolean IsModelOk()
	{
		return m_isLoadModel;
	}
	
	void SetClassName(List<String> className)
	{
	    m_className.clear();
	    m_className.addAll(className);
	}
}
