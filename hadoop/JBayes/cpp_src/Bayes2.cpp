#include "comment.h"

#include "Bayes2.h"
#include "FileSystem.h"
#include <string.h>

#include <algorithm>
using namespace std;

//#define __LINUX_DBG


int CBayes::BYSetClassList4(vector< vector<int> > vclass)
{
	vector< vector<int> >::iterator pIter = vclass.begin();
	int i = 0;

	while(pIter != vclass.end())
	{
		m_ClassList.insert(make_pair(i, *pIter));
		++pIter;
		++i;
	}

	return 0;
}


int CBayes::BYSelectAttributeFromClassFile4(TFIDFMatrix tfidf, 
								   vector< vector<int> > vclass, 
								   vector< vector<float> >& classattribute)
{
	int Kn = 0;
	int index = 0;

	Kn = vclass.size();

	vector< vector<int> >::iterator pvclass = vclass.begin();
	while (pvclass != vclass.end())
	{
		map<float, float> ClassAttr;
		vector<float> oneClassAttr;
      int file_num=0;
		vector<int>::iterator fileid = pvclass->begin();
		while(fileid != pvclass->end())
		{
			TFIDFMatrix::iterator pFile = tfidf.begin();
          if(*fileid >= tfidf.size())
            {
                ++fileid;
                ++file_num;
                break;
            }
			pFile += *fileid;

			multimap<float, float> mMap;

			MapToMultimap(*pFile, mMap);
         ReverseSelectAttrFromMultimap2(mMap, oneClassAttr);

			++fileid;
         ++file_num;
		}
      m_current_file_num[index] = file_num;

		sort(oneClassAttr.begin(), oneClassAttr.end());
		vector<float>::iterator new_last =  unique(oneClassAttr.begin(), oneClassAttr.end());
		vector<float> oneUseClassAttr(new_last-oneClassAttr.begin());
		copy(oneClassAttr.begin(), new_last, oneUseClassAttr.begin());
		classattribute.push_back(oneUseClassAttr);

		m_ClassList.insert(make_pair(index,*pvclass));
		++index;
		++pvclass;
	}

	return 0;
}


int CBayes::BYSetClassAttribute4(int ClassId, vector<float> classatrribute)
{
	map<int , vector<float> >::iterator pIter =  m_ClassAttrSet.find(ClassId);
	if (pIter != m_ClassAttrSet.end())
	{
		return -1;
	}

	m_ClassAttrSet.insert(make_pair(ClassId, classatrribute));
	return 0;
}


int CBayes::BYCreateWordProbabilityInClass4(ContentMatrix& content, int ClassId, map<string, float>& wordPro)
{

	float ClassSumWords = 0;
	float Words = 0;
	vector<int>classidset;


	map< int, vector<int> >::iterator  pClassId= m_ClassList.find(ClassId);
	if (pClassId == m_ClassList.end())
	{
		return -1;
	}
	classidset = pClassId->second;

	vector<int>::iterator csSumWords = classidset.begin();
	while(csSumWords != classidset.end())
	{
		ContentMatrix::iterator pFileSumWords = content.begin();

        if(*csSumWords >= content.size())
        {
            ++csSumWords;
            break;
        }
		pFileSumWords+= *csSumWords;


		ClassSumWords+= pFileSumWords->size();

		++csSumWords;
	}



	map<int , vector<float> >::iterator pClassAttr = m_ClassAttrSet.find(ClassId);
	if (pClassAttr == m_ClassAttrSet.end())
	{
		return -1;
	}

	vector<float>::iterator pAttr = pClassAttr->second.begin();
	int statistics = 0;
	cout<<"创建模型： 生成关键词概率....."<<endl;
	while(pAttr != pClassAttr->second.end())
	{
		Words = 0;

		vector<int>::iterator id = classidset.begin();
		while(id != classidset.end())
		{
			ContentMatrix::iterator pFile = content.begin();
            if(*id >= content.size())
            {
                ++id;
                break;
            }
			pFile += *id;

			vector<float>::iterator pWord = pFile->begin();
			while(pWord != pFile->end())
			{
				if (*pWord == *pAttr)
				{
					++Words;
				}
				++pWord;
			}
			//Words += count(pFile->begin(), pFile->end(), *pAttr);

			++id;
		}

		float p = Words / ClassSumWords;

		string word;
		//FindWordsById(IdToWords, *pAttr, word);
		GetWordById(*pAttr, word);

		//of <<word.c_str()<<":"<<p<<endl;
		wordPro.insert(make_pair(word, p));

		++pAttr;
		++statistics;
		cout<<statistics<<" ";
	}
	cout<<endl;

	return 0;
}


int CBayes::CalculateSimilaritryRange4(vector< StrVec > DatawordMatrix, 
							  int ClassId, 
							  map<string, float>& wordPro,
							  float& MinSimilarity,
							  float& MaxSimilarity
							  )
{
	vector<int>classidset;
	map< int, vector<int> >::iterator  pClassId= m_ClassList.find(ClassId);
	if (pClassId == m_ClassList.end())
	{
		return -1;
	}
	classidset = pClassId->second;

	MinSimilarity = 1;
	MaxSimilarity = 0;
	vector<int>::iterator pFile = classidset.begin();
	while (pFile != classidset.end())
	{
		float Similarity;
		map<string, float> ctWordsList;
        if(*pFile >= DatawordMatrix.size())
        {
            ++pFile;
            break;
        }

		StrVec dm = DatawordMatrix[*pFile];
		CalculateWordProInFile4(dm, ctWordsList);
		CalculateFileSimilarityInModel4(wordPro, ctWordsList, Similarity);

		if (Similarity < MinSimilarity)
		{
			MinSimilarity = Similarity;
		}

		if (Similarity > MaxSimilarity)
		{
			MaxSimilarity = Similarity;
		}

		++pFile;
	}

	return 0;
}

int CBayes::BYCreateClassModel4(ContentMatrix& content, 
						vector< StrVec >& DatawordMatrix, 
						int ClassId, 
						string ModelFile, float threshold)
{
	map<string, float> wordPro;
	BYCreateWordProbabilityInClass4(content, ClassId, wordPro);

	float MinSimilarity = 0;
	float MaxSimilarity = 0;
	CalculateSimilaritryRange4(DatawordMatrix, 
		ClassId, 
		wordPro,
		MinSimilarity,
		MaxSimilarity
		);

	CModel* pModel = new CFileModel(ModelFile);

    if(m_className.size() == 0)
    {
	    string mn = "test";
	    pModel->setmodulename(mn);
    }
    else
    {
       pModel->setmodulename(m_className[ClassId]);
    }
	float thresholdrange[2];
	thresholdrange[0] = MinSimilarity;
	thresholdrange[1] = MaxSimilarity;
	pModel->setthresholdrange(thresholdrange);

    if(threshold > 0)
	    pModel->setthreshold(threshold);
    else
	    pModel->setthreshold(MinSimilarity);

	pModel->setwordpro(wordPro);
   pModel->m_filenumber = m_current_file_num[ClassId];

	pModel->savemodule();

	delete pModel;

	return 0;
}

bool EnumerateModel2(char* lpFileOrPath, vector<string>& FileList)
{
	//char* pMd = ".md";
	char pMd[32] = {0};
	strcpy(pMd, ".md");
	const char* pDot = strrchr(lpFileOrPath, '.');
	if (pDot == NULL)
	{
		return true;
	}

	if (memcmp(pDot, pMd, 3) != 0)
	{
		return true;
	}

	FileList.push_back(lpFileOrPath);

	return true;
}


int CBayes::BYLoadClassModel4(vector<string>& Models)
{
	vector<string>::iterator pModelFile = Models.begin();
	int ok_n = 0;
	while(pModelFile != Models.end())
	{
		CModel* pModel = new CFileModel();
		try
		{
			pModel->loadmodule(*pModelFile);
			m_ModelList.push_back(pModel);
			m_classNM.insert(make_pair(pModel->getmodulename(), pModel));
			++ok_n;
		}
		catch(...)
		{
			string msg = "model file value type error:";
			//msg += *pModelFile;
			//syslog(LOG_ERR, msg.c_str());
		}

		++pModelFile;
		if(ok_n > 1)
			m_isLoadModel = true;
	}

	return 0;
}

int CBayes::BYPredict4(string Content, vector<float>& ProbabilityList)
{
	StrVec Words;
	float Probability;
	//Dic->SplitWord(Content, Words);

	vector< map<string, float> >::iterator pModel = m_Model.begin();

	while(pModel != m_Model.end())
	{
		Probability = 0;
		StrVec::iterator pOneWord = Words.begin();
		while(pOneWord != Words.end())
		{
			map<string, float>::iterator pModelWord = pModel->find(*pOneWord);
			if (pModelWord != pModel->end())
			{
				Probability += pModelWord->second;
			}
			++pOneWord;
		}

		//Probability = Probability / Words.size();

		ProbabilityList.push_back(Probability);
		++pModel;
	}
	return 0;
}

int CBayes::SetThreshold4(float threshold)
{
	//g_threshold = threshold;
	return 0;
}

int CBayes::CalculateWordProInFile4(StrVec& Words, map<string, float>& ctWordsList)
{
	float ctCount = Words.size();
	StrVec::iterator pOneWord = Words.begin();
	while(pOneWord != Words.end())
	{
		map<string, float>::iterator pctList = ctWordsList.find(*pOneWord);
		if (pctList == ctWordsList.end())
		{
			ctWordsList.insert(make_pair(*pOneWord, 1/ctCount));
		}
		else
		{
			pctList->second += 1/ctCount;
		}

		++pOneWord;
	}

	return 0;
}

int CBayes::CalculateFileProInModel4(int ModelId, StrVec Words, float& Probability, vector<string>& KeyWords)
{
	float lProbability = 0;

	vector< map<string, float> >::iterator pModel = m_Model.begin();
	pModel+=ModelId;

	StrVec::iterator pWords = Words.begin();
	while(pWords != Words.end())
	{
		map<string, float>::iterator pModelWord = pModel->find(*pWords);
		if (pModelWord != pModel->end())
		{
			KeyWords.push_back(*pWords);
			lProbability += pModelWord->second;
		}
		++pWords;
	}

	//Probability = Probability / Words.size();
	
	Probability = lProbability;
	return 0;
}

int CBayes::CalculateFileSimilarityInModel4(map<string, float> Model, map<string, float> ctWordsList, float& Similarity)
{
	float lSimilarity = 0;

	map<string, float>::iterator pWords = ctWordsList.begin();
	while(pWords != ctWordsList.end())
	{
		map<string, float>::iterator pModelWord = Model.find(pWords->first);
		if (pModelWord != Model.end())
		{
			lSimilarity += pWords->second;
		}
		++pWords;
	}

	//Probability = Probability / Words.size();

	Similarity = lSimilarity;
	return 0;
}

bool CBayes::IsAttractiveContent4(vector<string> Words, string &uid, string& level, string& ClassName, float& oSimilarity, vector<string>& KeyWords)
{
	bool bRtn = false;
	vector< vector<float> > ProbabilityList;

	if (!m_isLoadModel)
	{
		return false;
	}

	float Probability = 0;
	float similarity = 0;

   if(Words.size() < 10)
      return false;

#ifdef __LINUX_DBG
	string dbgSplitWord="SplitWord:";
	StrVec::iterator pDbgWord = Words.begin();
	int splitcount = 0;
	while (pDbgWord != Words.end())
	{
		string strNum;
		ostringstream os;
		os<<splitcount;
		strNum = os.str();
		dbgSplitWord += strNum;
		dbgSplitWord += " ";
		dbgSplitWord += *pDbgWord;
		dbgSplitWord += "--";

		++splitcount;
		++pDbgWord;
	}
	//syslog(LOG_DEBUG, dbgSplitWord.c_str());
#endif

	map<string, float> ctWordsList;
	CalculateWordProInFile4(Words, ctWordsList);
	
	//vector< map<string, float> >::iterator pModel = g_Model.begin();
	vector<CModel*>::iterator pModel = m_ModelList.begin();
	while(pModel != m_ModelList.end())
	{
		Probability = 0;
		similarity = 0;
		map<string, float>* pModePro = (*pModel)->getwordpro();

#ifdef __LINUX_DBG
		string dbginfo = "bayes mode:";
		map<string, float>::iterator pModelWord_Dbg = pModePro->begin();
		while(pModelWord_Dbg != pModePro->end())
		{
			dbginfo += pModelWord_Dbg->first;
			dbginfo += " ";
			++pModelWord_Dbg;
		}
		//syslog(LOG_DEBUG, dbginfo.c_str());
#endif

#ifdef __LINUX_DBG
		dbginfo="unmatch:";
#endif
		map<string, float>::iterator pctList = ctWordsList.begin();
		while(pctList != ctWordsList.end())
		{
			map<string, float>::iterator pModelWord = pModePro->find(pctList->first);
			if (pModelWord != pModePro->end())
			{
				KeyWords.push_back(pctList->first);
				Probability += pModelWord->second;
				similarity += pctList->second;
			}
#ifdef __LINUX_DBG
			else
			{
				dbginfo+=pctList->first;
				dbginfo+=" ";
			}
#endif
			++pctList;
		}
#ifdef __LINUX_DBG
		//syslog(LOG_DEBUG, dbginfo.c_str());
#endif

		//Probability = Probability / Words.size();

      Probability *= (*pModel)->m_filenumber;
		vector<float> oneResult;
		oneResult.push_back(Probability);
		oneResult.push_back(similarity);
		ProbabilityList.push_back(oneResult);

		++pModel;
	}

	vector< vector<float> >::iterator pIter = ProbabilityList.begin();
	float MaxProb = 0, MaxIndex=0, i=0;
	while(pIter != ProbabilityList.end())
	{
		if (MaxProb < (*pIter)[0])
		{
			MaxProb = (*pIter)[0];
			MaxIndex = i;
		}
		++pIter;
		++i;
	}
	
	vector<float> oneProb =  ProbabilityList[MaxIndex];
	if (oneProb[1] >= ((CModel*)m_ModelList[MaxIndex])->getthreshold() )
	{
		oSimilarity = oneProb[1];
		ClassName = ((CModel*)m_ModelList[MaxIndex])->getmodulename();
		uid = ((CModel*)m_ModelList[MaxIndex])->getuid();
		level = ((CModel*)m_ModelList[MaxIndex])->getlevel();
		bRtn = true;
	}

	return bRtn;
}

CBayes::~CBayes()
{
	delete m_File;

	vector<CModel*>::iterator pModel = m_ModelList.begin();
	while(pModel != m_ModelList.end())
	{
		delete(*pModel);
		++pModel;
	}
}

bool CBayes::IsModelOk()
{
	return m_isLoadModel;
}

void CBayes::SetClassName(vector<string>& className)
{
    m_className.resize(0);
    m_className = className;
}
