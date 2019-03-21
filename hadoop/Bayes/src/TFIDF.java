import java.util.ArrayList;
import java.util.HashMap;
import java.util.IdentityHashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class TFIDF {
	
	void MapToIdentityHashMap(Map<Float, Float> src, IdentityHashMap<Float, Float> des)
	{
		Iterator<Float> iter = src.keySet().iterator();
		while(iter.hasNext())
		{
			Float k = iter.next();
			des.put(src.get(k), k);
		}
	}
	
	/*
	TF-IDF算法，用来计算一个词汇的重要程度，原理是一个词汇在文章内出现的次数越多权重越高，
	但在语料库中出现的次数越多权重越低。 这里，我们将所有文件的词组集合作为语料库。
	*/
	void TFIDFcal(List<List<Float>> content, List<Map<Float,Float>> tfidf)
	{
		int wordsum = 0;
		int i = 0;
		List<Map<Float,Float>> InsideFrequency = new ArrayList<Map<Float,Float>>();//内部词频矩阵
		Map<Float,Float> TotalityFrequency = new HashMap<Float,Float>();//总体词频表
		Map<Float,Float> CorpusFrequency = new HashMap<Float,Float>();//词汇n篇文章中
	
		//统计词汇总数和文件内词汇频率
		Iterator<List<Float>> pC = content.iterator();
		while(pC.hasNext())
		{
			//统计文件词汇频率
			List<Float> record = new ArrayList<Float>();
			List<Float> pWord = pC.next();
			Map<Float,Float> fileinside = new HashMap<Float,Float>();
	
			for (i=0; i< pWord.size(); ++i)
			{//遍历一篇文章
				Float pIter = fileinside.get(pWord.get(i));
				if(pIter == null)
					fileinside.put(pWord.get(i), new Float(1));
				else
					fileinside.put(pWord.get(i), pIter.floatValue() + 1);
				
	
				//总体词频
				pIter = TotalityFrequency.get(pWord.get(i));
				if (pIter == null)
				{
					
					TotalityFrequency.put(pWord.get(i), new Float(1));
				}
				else
				{
					TotalityFrequency.put(pWord.get(i), pIter.floatValue() + 1);
				}
				

	
				//预料频率
				pIter = CorpusFrequency.get(pWord.get(i));
				
				if (pIter == null)
				{
					CorpusFrequency.put(pWord.get(i), new Float(1));
					record.add(pWord.get(i));
				}
				else
				{
					Float pRecord = record.get(pWord.get(i).intValue());
					if (pRecord == null)
					{
						CorpusFrequency.put(pWord.get(i), pIter + 1);
						record.add(pWord.get(i));
					}
				}
			}
	
			InsideFrequency.add(fileinside);
	
			//统计总词汇量
			wordsum += pWord.size();
		}
		
		//计算TF-IDF值
		pC = content.iterator();
		Iterator<Map<Float,Float>> pFrequ = InsideFrequency.iterator();
		while(pC.hasNext())
		{
			double TF = 0;
			double IDF = 0;
			Map<Float,Float> oneP = pFrequ.next();
			Iterator<Float> onePI = oneP.keySet().iterator();
			while(onePI.hasNext())
			{//遍历一篇文章所有词汇
				Float onePIT = onePI.next();
				Float pCorpusNum = CorpusFrequency.get(onePIT);
				TF = oneP.get(onePIT) / pC.next().size(); //词频（TF）=某关键词出现次数/文章中关键词总数
				IDF = Math.log(new Double(content.size())/new Double((pCorpusNum.floatValue() +1))); //逆文档频率（IDF）=log（语料库文档总数/（包含该词的文档数+1））
				
				oneP.put(onePIT.floatValue(), new Float(TF*IDF));
			}
		}
	
		tfidf = InsideFrequency;
	}
	
	//
	//计算文章间的TFIDF相似度
	//用于计算文章A与哪篇文章更接近
	//本函数原理：两篇文章，词汇重复越多相似度越高， 词汇的TF-IDF越高相似度越高
	//
	void TFIDFDistance(List<Map<Float,Float>> tfidf, List<List<Float>> distance)
	{
		//距离计算：A与B，取相同词汇TFIDF之和
		int i=0;
		
		
		Iterator<Map<Float,Float>> iter  = tfidf.iterator();
		while(iter.hasNext())
		{
			i=0;
			List<Float> essayDistance = new ArrayList<Float>();
			Map<Float,Float> iterT = iter.next();
			
			
			Iterator<Map<Float,Float>> iterCmp  = tfidf.iterator();
			while(iterCmp.hasNext())
			{
				Map<Float, Float> iterCmpT = iterCmp.next();
				essayDistance.add(new Float(0));
				if (iter == iterCmp)
				{//自己与自己的相似度记为0
					;
				}
				else
				{//计算与其他文章的相似度
					
					if (iterT.size() < iterCmpT.size())
					{
						
						Iterator<Float> iterMap = iterT.keySet().iterator();
						while(iterMap.hasNext())
						{
							Float tmp = iterMap.next();
							Float pFind = iterCmpT.get(tmp);
							if(pFind != null)
								essayDistance.add(i, pFind + iterT.get(iterMap));
						}
					}
					else
					{
						Iterator<Float> iterMap = iterCmpT.keySet().iterator();
						while(iterMap.hasNext())
						{
							Float tmp = iterMap.next();
							Float pFind = iterT.get(tmp);
							if(pFind != null)
								essayDistance.add(i, pFind + iterCmpT.get(iterMap));
						}
					}
				}

				++i;
			}
	
			distance.add(essayDistance);
		}
	}
	
	public static void ReverseSelectAttrFromIdentityHashMap(IdentityHashMap<Float, Float> mMap, List<Float> Attr)
	{
	    if( 0 == mMap.size() )
	    {
	        return;
	    }
	
	    Float sum = new Float(0);
	    Iterator<Float> tmp_it = mMap.keySet().iterator();
	    while(tmp_it.hasNext())
	        sum += tmp_it.next();
	    sum /= mMap.size();
	
	    Iterator<Float> mIter = mMap.keySet().iterator();
	    while(mIter.hasNext())
	    {
	    	Float mIterT = mIter.next();
	        if(mIterT > sum)
	            Attr.add(mMap.get(mIterT));
	    }
	
	    //根据权重判断
	    mIter = mMap.keySet().iterator();
	    while(mIter.hasNext())
	    {
	    	Float mIterT = mIter.next();
	        if (mIterT.floatValue() > 0.015)
	            Attr.add(mMap.get(mIterT));
	    }
	}
	
	//计算n个节点的均值，生成一个平均值
	void TFIDFMeans(List<Map<Float,Float>> nodeset, Map<Float, Float> mean)
	{
		//Integer n= nodeset.size();
		int rep=0;
		IdentityHashMap<Float, Float> temp = new IdentityHashMap<Float, Float>();
	
		Iterator<Map<Float,Float>> element = nodeset.iterator();
	
		while(element.hasNext())
		{
			Map<Float,Float> elementT = element.next();
			
			if (0 == elementT.size())
			{
				continue;
			}
	
			//测试 使用1~15个关键词
			rep = elementT.size();//取三分之一
			if (rep>30)
			{
				rep = 30;//每篇文章只取30个关键期
			}
	
			//按TFIDF排序 temp
			MapToIdentityHashMap(elementT, temp);
			Iterator<Float> mIter = temp.keySet().iterator();
			//mIter--;
			while(--rep > 0)
			{
				Float tmp = mIter.next();
				Float pFind = mean.get(temp.get(tmp));
				if (pFind != null)
				{
					if (pFind < tmp)
					{
						mean.put(temp.get(tmp), tmp);
					}
				}
				else
				{
					mean.put(temp.get(tmp), tmp);
				}
			}
		}
	
	}
	
	//计算中心点到各个节点的距离
	void TFIDFMeansDistance(Map<Float, Float> mean, List<Map<Float,Float>> nodeset,  List<Float> distance)
	{
		Iterator<Map<Float,Float>> element = nodeset.iterator();
		while(element.hasNext())
		{
			float fDistance = 0;
			Map<Float,Float> mdElementM = element.next();
			Iterator<Float> mdElement = mdElementM.keySet().iterator();
			while(mdElement.hasNext())
			{
				Float mdElementT = mdElement.next();
				Float mdFind = mean.get(mdElementT);
				if (mdFind != null)
				{
					fDistance += mdElementM.get(mdElementT);
				}
			}
	
			distance.add(fDistance);
		}
	
	}
	
	void KMeansInitClass(int K, List<Map<Float,Float>> tfidf, List<Integer> rclass)
	{
		List<List<Float>> distance = new ArrayList<List<Float>>();
		int currentClass = 0;
		int tempClass = 0;
		int i,j,x;
		float mix = 10000;
	
		TFIDFDistance(tfidf, distance);
		rclass.add(0);//第一个0
	
	
		for (i=0; i<K-1; ++i)
		{
			Iterator<Float> pEle = distance.get(currentClass).iterator();
			x=0;
			while(pEle.hasNext())
			{
				Float tmp = pEle.next();
				if (tmp.floatValue() < mix &&
					x != currentClass &&
					rclass.get(x) == null)
				{
					mix = tmp.floatValue();
					tempClass = x;
				}
	
				++x;
			}
	
			mix = 10000;
			currentClass = tempClass;
			rclass.add(currentClass);
		}
	}
	
	void TFIDFKMeans(List<Map<Float,Float>> tfidf, Integer k, List<Integer> vclass, List<Map<Float,Float>> centAttri)
	{
		//time_t Time;
		int i=0, j=0;
		List<List<Map<Float,Float>>> clusters = new ArrayList<List<Map<Float,Float>>>();//记录
		List<Integer> clusterAssignments = new ArrayList<Integer>();//所属簇
		List<Integer> nearestCluster = new ArrayList<Integer>();//动态计算得到的最近簇
		float nearestDistance;
		List<Map<Float,Float>> coordinates = new ArrayList<Map<Float,Float>>();//簇中心
		List<List<Float>> CtoEdistance = new ArrayList<List<Float>>();//中心到各个元素的距离
		Integer nElement = tfidf.size();
	
		//clusterAssignments.resize(nElement);
		//nearestCluster.resize(nElement);
	
		//固定分类
		List<Integer> rclass = new ArrayList<Integer>();
		KMeansInitClass(k, tfidf, rclass);
		for (i=0; i<k; ++i)
		{
			Map<Float,Float> initNode = tfidf.get(rclass.get(i));
			List<Map<Float,Float>> initMatrix = new ArrayList<Map<Float,Float>>();
			initMatrix.add(initNode);
			clusters.add(i, initMatrix);
		}
	
		while(true)
		{
			//更新簇中心
			for (i=0; i<k; ++i)
			{
				TFIDFMeans(clusters.get(i),coordinates.get(i));
			}
	
			//计算每个数据和每个聚类中心的距离
			for (i=0; i<k; ++i)
			{
				TFIDFMeansDistance(coordinates.get(i), tfidf, CtoEdistance.get(i));
			}
	
			//计算每个数据离哪个聚类最近，然后将他们归为一类
			for(i=0; i<nElement; ++i)
			{
				nearestDistance = 0;
				nearestCluster.set(i, 0);
				for(j=0; j<k; ++j)
				{
					List<Float> nearest;
					nearest = CtoEdistance.get(j);
					if(nearestDistance < nearest.get(i))
					{
						nearestDistance = nearest.get(i);
						nearestCluster.set(i, j);
					}
				}
	
				//i属于nearestCluster簇
			}
	
			//4、比较每个数据最近的聚类是否就是它所属的聚类
			//如果全相等表示所有的点已经是最佳距离了，直接返回；
			for(i=0, j=0; i<nElement; ++i)
			{
				if (nearestCluster.get(i) == clusterAssignments.get(i))
				{
					++j;
				}
			}
	
			if (j == nElement)
			{//完全匹配
				vclass = clusterAssignments;
				centAttri = coordinates;
				break;
			}
	
	
			//5、否则需要重新调整资料点和群聚类的关系，调整完毕后再重新开始循环；
			//需要修改每个聚类的成员和表示某个数据属于哪个聚类的变量
			coordinates.clear();
			clusters.clear();
			//clusters.resize(k);
			//coordinates.resize(k);
			for(i=0; i<nElement; ++i)
			{
				Iterator<List<Map<Float,Float>>> pClusters = clusters.iterator();
				for (j=0; j<nearestCluster.get(i); ++j)
				{
					pClusters.next();
				}
	
				clusterAssignments.set(i, nearestCluster.get(i) );
				pClusters.next().add(tfidf.get(i));
			}
		}
	}
}
