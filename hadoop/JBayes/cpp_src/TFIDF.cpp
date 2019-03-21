// TFIDF.cpp : Defines the entry point for the console application.
//

#include "comment.h"
//#include "File.h"
//#include "convert.h"

#define _debug_kmeans


/*
TF-IDF算法，用来计算一个词汇的重要程度，原理是一个词汇在文章内出现的次数越多权重越高，
但在语料库中出现的次数越多权重越低。 这里，我们将所有文件的词组集合作为语料库。
*/
void TFIDF(ContentMatrix& content, TFIDFMatrix& tfidf)
{
	int wordsum = 0;
	unsigned int i = 0;
	TFIDFMatrix InsideFrequency;//内部词频矩阵
	map<float,float> TotalityFrequency;//总体词频表
	map<float,float> CorpusFrequency;//词汇n篇文章中

	//统计词汇总数和文件内词汇频率
	ContentMatrix::iterator pC = content.begin();
	while(pC != content.end())
	{
		//统计文件词汇频率
		vector<float> record;
		vector<float>::iterator pWord = pC->begin();
		map<float,float> fileinside;

		for (i=0; i< pC->size(); ++i)
		{//遍历一篇文章
			map<float,float>::iterator pIter;
			pIter = fileinside.find(pWord[i]);
			if (pIter != fileinside.end())
			{
				pIter->second++;
			}
			else
			{
				fileinside.insert(make_pair(pWord[i], 1));
			}

			//总体词频
			pIter = TotalityFrequency.find(pWord[i]);
			if (pIter != TotalityFrequency.end())
			{
				pIter->second++;
			}
			else
			{
				TotalityFrequency.insert(make_pair(pWord[i], 1));
			}

			//预料频率
			pIter = CorpusFrequency.find(pWord[i]);
			
			if (pIter != CorpusFrequency.end())
			{
				vector<float>::iterator pRecord = find(record.begin(), record.end(), pWord[i]);
				if (pRecord != record.end())
				{
				}
				else
				{
					pIter->second++;
					record.push_back(pWord[i]);
				}
				
			}
			else
			{
				CorpusFrequency.insert(make_pair(pWord[i], 1));
				record.push_back(pWord[i]);
			}
		}

		InsideFrequency.push_back(fileinside);

		//统计总词汇量
		wordsum += pC->size();
		++pC;
	}

	//计算TF-IDF值
	pC = content.begin();
	TFIDFMatrix::iterator pFrequ = InsideFrequency.begin();
	while(pC != content.end())
	{
		float TF = 0;
		float IDF = 0;
		map<float,float>::iterator oneP = pFrequ->begin();
		while(oneP != pFrequ->end())
		{//遍历一篇文章所有词汇
			map<float,float>::iterator pCorpusNum = CorpusFrequency.find(oneP->first);
			TF = oneP->second / pC->size(); //词频（TF）=某关键词出现次数/文章中关键词总数
			IDF = log(content.size()/(pCorpusNum->second +1))   ; //逆文档频率（IDF）=log（语料库文档总数/（包含该词的文档数+1））
			
			oneP->second = TF*IDF;
			++oneP;
		}

		++pC;
		++pFrequ;
	}

	tfidf = InsideFrequency;
}

//
//计算文章间的TFIDF相似度
//用于计算文章A与哪篇文章更接近
//本函数原理：两篇文章，词汇重复越多相似度越高， 词汇的TF-IDF越高相似度越高
//
void TFIDFDistance(TFIDFMatrix& tfidf, vector<vector<float> >& distance)
{
	//距离计算：A与B，取相同词汇TFIDF之和
	int i=0;
	
	TFIDFMatrix::iterator iter = tfidf.begin();
	while(iter != tfidf.end())
	{
		i=0;
		vector<float> essayDistance;

		TFIDFMatrix::iterator iterCmp = tfidf.begin();
		while(iterCmp != tfidf.end())
		{
			essayDistance.push_back(0);
			if (iter == iterCmp)
			{//自己与自己的相似度记为0
				;
			}
			else
			{//计算与其他文章的相似度
				
				if (iter->size() < iterCmp->size())
				{
					map<float,float>::iterator itermap = iter->begin();
					while(itermap != iter->end())
					{
						map<float,float>::iterator pFind = iterCmp->find(itermap->first);
						if (pFind != iterCmp->end())
						{
							essayDistance[i] += (pFind->second+itermap->second);
						}

						++itermap;
					}
				}
				else
				{
					map<float,float>::iterator itermap = iterCmp->begin();
					while(itermap != iterCmp->end())
					{
						map<float,float>::iterator pFind = iter->find(itermap->first);
						if (pFind != iter->end())
						{
							essayDistance[i] += (pFind->second+itermap->second);
						}

						++itermap;
					}
				}
			}

			++iterCmp;
			++i;
		}

		distance.push_back(essayDistance);
		++iter;
	}
}

void MapToMultimap(map<float, float>& fMap, multimap<float, float>& mMap)
{
	map<float, float>::iterator pIter = fMap.begin();
	while(pIter != fMap.end())
	{
		mMap.insert(make_pair(pIter->second, pIter->first));

		++pIter;
	}

}

void ReverseSelectAttrFromMultimap(multimap<float, float>& mMap, vector<float>& Attr , int n)
{
	if( 0 == mMap.size() )
	{
		return;
	}
	//按百分比
	multimap<float, float>::iterator mIter = mMap.end();
	mIter--;
	while(n>0)
	{
		Attr.push_back(mIter->second);

		mIter--;
		n--;
	}

	//根据权重判断
	mIter = mMap.end();
	mIter--;
	while(mIter != mMap.begin())
	{
		if (mIter->first < 0.015)
		{
			break;
		}
		Attr.push_back(mIter->second);

		mIter--;
	}
}

void ReverseSelectAttrFromMultimap2(multimap<float, float>& mMap, vector<float>& Attr)
{
    if( 0 == mMap.size() )
    {
        return;
    }

    float sum = 0;
    multimap<float, float>::iterator tmp_it = mMap.begin();
    while(tmp_it != mMap.end())
    {
        sum += (*tmp_it).first;
        ++tmp_it;
    }
    sum /= mMap.size();

    multimap<float, float>::iterator mIter = mMap.begin();
    while(mIter != mMap.end())
    {
        if(mIter->first > sum)
            Attr.push_back(mIter->second);
        ++mIter;
    }

    //根据权重判断
    mIter = mMap.begin();
    while(mIter != mMap.end())
    {
        if (mIter->first > 0.015)
            Attr.push_back(mIter->second);
        ++mIter;
    }
}

//计算n个节点的均值，生成一个平均值
void TFIDFMeans(TFIDFMatrix& nodeset, map<float, float>& mean)
{
	//int n= nodeset.size();
	int rep=0;
	multimap<float, float> temp;

	TFIDFMatrix::iterator element = nodeset.begin();

	while(element != nodeset.end())
	{
		if (0 == element->size())
		{
			++element;
			continue;
		}
		//按百分比取关键词
		//rep = element->size()/3;//取三分之一
		//if (rep==0)
		//{
		//	rep = 1;//最少取一个关键字
		//}

		//测试 使用1~15个关键词
		rep = element->size();//取三分之一
		if (rep>30)
		{
			rep = 30;//每篇文章只取30个关键期
		}

		//按TFIDF排序 temp
		MapToMultimap(*element, temp);
		multimap<float, float>::iterator mIter = temp.end();
		mIter--;
		while(--rep)
		{
			map<float, float>::iterator pFind = mean.find(mIter->second);
			if (pFind != mean.end())
			{
				if (pFind->second < mIter->first)
				{
					pFind->second = mIter->first;
				}
			}
			else
			{
				mean.insert(make_pair(mIter->second, mIter->first));
			}
			
			--mIter;
		}

		++element;
	}

}

//计算中心点到各个节点的距离
void TFIDFMeansDistance(map<float, float>& mean, TFIDFMatrix& nodeset,  vector<float>& distance)
{
	multimap<float, float> temp;
	
	TFIDFMatrix::iterator element = nodeset.begin();
	while(element != nodeset.end())
	{
		float fDistance = 0;
		map<float,float>::iterator mdElement = element->begin();
		while(mdElement != element->end())
		{
			map<float,float>::iterator mdFind = mean.find(mdElement->first);
			if (mdFind != mean.end())
			{
				fDistance += (mdElement->second);
			}

			++mdElement;
		}

		distance.push_back(fDistance);

		++element;
	}

}

void KMeansInitClass(int K, TFIDFMatrix tfidf, vector<int>& rclass)
{
	vector<vector<float> > distance;
	int currentClass = 0;
	int tempClass = 0;
	int i,j,x;
	float mix = 10000;

	TFIDFDistance(tfidf, distance);
	rclass.push_back(0);//第一个0


	for (i=0; i<K-1; ++i)
	{
		vector<vector<float> >::iterator pDistance = distance.begin();
		j=0;
		while(j<currentClass)
		{
			++pDistance;
			++j;
		}

		vector<float>::iterator pEle = pDistance->begin();
		x=0;
		while(pEle != pDistance->end())
		{
			if (*pEle < mix &&
				x != currentClass &&
				find(rclass.begin(), rclass.end(), x) == rclass.end())
			{
				mix = *pEle;
				tempClass = x;
			}

			++pEle;
			++x;
		}

		mix = 10000;
		currentClass = tempClass;
		rclass.push_back(currentClass);

		
	}
}

void TFIDFKMeans(TFIDFMatrix& tfidf, int k, vector<int>& vclass, TFIDFMatrix& centAttri)
{
	//time_t Time;
	int i=0, j=0;
	vector<TFIDFMatrix> clusters(k);//记录
	vector<int> clusterAssignments;//所属簇
	vector<int> nearestCluster;//动态计算得到的最近簇
	float nearestDistance;
	TFIDFMatrix coordinates(k);//簇中心
	vector<vector<float> > CtoEdistance(k);//中心到各个元素的距离
	int nElement = tfidf.size();

	clusterAssignments.resize(nElement);
	nearestCluster.resize(nElement);

	//随机初始化k个簇
	//localtime(&Time);
	//srand(Time);
	//for(i=0; i<k; ++i)
	//{
	//	map<float,float> initNode = tfidf[rand()%nElement];
	//	TFIDFMatrix initMatrix;
	//	initMatrix.push_back(initNode);
	//	clusters[i] = initMatrix;
	//}
	//固定分类
	vector<int> rclass;
	KMeansInitClass(k, tfidf, rclass);
	for (i=0; i<k; ++i)
	{
		map<float,float> initNode = tfidf[rclass[i]];
		TFIDFMatrix initMatrix;
		initMatrix.push_back(initNode);
		clusters[i] = initMatrix;
	}

	while(true)
	{
		//更新簇中心
		for (i=0; i<k; ++i)
		{
			TFIDFMeans(clusters[i],coordinates[i]);
		}

		//计算每个数据和每个聚类中心的距离
		for (i=0; i<k; ++i)
		{
			TFIDFMeansDistance(coordinates[i], tfidf, CtoEdistance[i]);
		}

		//计算每个数据离哪个聚类最近，然后将他们归为一类
		for(i=0; i<nElement; ++i)
		{
			nearestDistance = 0;
			nearestCluster[i] = 0;
			for(j=0; j<k; ++j)
			{
				vector<float> nearest;
				nearest = CtoEdistance[j];
				if(nearestDistance < nearest[i])
				{
					nearestDistance = nearest[i];
					nearestCluster[i] = j;
				}
			}

			//i属于nearestCluster簇
		}

		//4、比较每个数据最近的聚类是否就是它所属的聚类
		//如果全相等表示所有的点已经是最佳距离了，直接返回；
		for(i=0, j=0; i<nElement; ++i)
		{
			if (nearestCluster[i] == clusterAssignments[i])
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
		clusters.resize(k);
		coordinates.resize(k);
		for(i=0; i<nElement; ++i)
		{
			vector<TFIDFMatrix>::iterator pClusters = clusters.begin();
			TFIDFMatrix::iterator clustersF;

			for (j=0; j<nearestCluster[i]; ++j)
			{
				++pClusters;
			}

			clusterAssignments[i] = nearestCluster[i];
			pClusters->push_back(tfidf[i]);
		}



	}


}


void ictclas30(string& input, StrVec& retWords)
{//分词算法，input为输入串,retWords为处理后所分开的单词,这里就简单化处理了，以空格符为分隔符进行分词
	//Dic->SplitWord(input, retWords);
}

int GetWordById(float id, string& word)
{
	
	return 0; //Dic->GetWord(id, word);
}

int GetIdByWord(string& word, float& id)
{
	return 0;//Dic->GetId(word, id);
}

int InsertWordAndId(string word, float id)
{
	return 0;//Dic->InsertWordId(word, id);
}

bool TFWordIdOk()
{
	return false;//Dic->WordIdOk();
}
