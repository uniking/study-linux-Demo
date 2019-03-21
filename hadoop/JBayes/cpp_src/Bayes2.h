#pragma once
#ifdef WIN32
#include <Windows.h>
#endif


#include "FileSystem.h"

#include "FileModel.h"

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
using namespace std;

#ifdef WIN32
#define DLL_EXPORT_API __declspec(dllexport)
#else
#define DLL_EXPORT_API
#endif


#define  ContentMatrix vector<vector<float> >
#define  TFIDFMatrix vector<map<float,float> > //(id,概率)
typedef vector<string> StrVec; //字符串向量
typedef vector<int> IntVec; //整数向量
typedef vector<vector<int> > Int2DVec;//整数二维向量
typedef vector<vector<double> > Double2DVec;//浮点数二维向量
typedef vector<double> DoubleVec;//浮点数向量


/*
原理：
           P(A|B)*P(B)
P(B|A) = --------------
             P(A)


*/
class DLL_EXPORT_API CBayes
{
public:
	map< int , vector<float> > m_ClassAttrSet;
	map< int, vector<int> > m_ClassList;
	vector<CModel*> m_ModelList;
	vector< map<string, float> > m_Model;
	bool m_isLoadModel;
	discover::File* m_File;
   vector<int> m_current_file_num;
   vector<string> m_className;

   map< string, CModel* > m_classNM;

public:
#ifdef WIN32
	CBayes(){m_File = new discover::winFile();  m_isLoadModel=false; m_current_file_num.resize(100);};
#else
	CBayes(){m_File = new discover::linuxFile(); m_isLoadModel=false; m_current_file_num.resize(100);};
#endif
	~CBayes();

	
	/*
	描述：
	从文件中抽取一些词组作为一个类属性集合

	参数：
	tfidf TFIDF运算结果，包含了文章中所有词的TFIDF值
	vclass 指定tfidf中的分类情况
	classatrribute 返回抽取的类属性

	返回值：
	0 表示成功
	*/
	int BYSelectAttributeFromClassFile4(TFIDFMatrix tfidf, 
		vector< vector<int> > vclass, 
		vector< vector<float> >& classattribute);

	/*
	描述：
	设置类的属性集合

	参数：
	ClassId 要设置的类id
	classatrribute 属性列表，这个列表用来生成类模型

	返回值：
	0 成功
	*/
	int BYSetClassAttribute4(int ClassId, vector<float> classatrribute);

	/*
	为指定的类生成类模型
	ClassId 类Id
	*/
	int BYCreateClassModel4(ContentMatrix& content, 
		vector< StrVec >& DatawordMatrix, 
		int ClassId, 
		string ModelFile, float threshold=0);

	/*
	描述：
	加载指定目录内的所有模型文件（一个类一个模型）

	参数：
	ModelDir 模型文件的目录

	返回值：
	0 成功
	*/
	int BYLoadClassModel4(string ModelDir);
	int BYLoadClassModel4(vector<string>& Models);


	/*
	描述：
	计算文件内容在每个模型中的概率

	参数：
	Content 要判断的文本内容
	ProbabilityList 返回在各个类中的概率
	*/
	int BYPredict4(string Content, vector<float>& ProbabilityList);

	/*
	描述：
	为判断是否为敏感文件设置阈值

	参数：
	threshold 阈值，所属类的概率超过此阈值将视为敏感文件

	返回值：
	0 表示成功
	*/
	int SetThreshold4(float threshold);

	/*
	判断是否是敏感信息，需要预先设置阈值
	Content 要判断的文本内容
	ClassName 返回所属的类名
	如果返回false，则说明不属于任何类
	*/
	bool IsAttractiveContent4(vector<string> Words, string &uid, string& level, string& ClassName, float& oSimilarity, vector<string>& KeyWords);
   
   


	/*
	描述：
	计算文件与指定模型的相似度， 一个文件所有词汇出现的概率之和为1， 返回值为模型与文件词组的交集，保证了交集词组概率之和在0-1范围内。

	参数：
	Model 模型文件
	ctWordsList 文件中每个词汇的概率
	Similarity 返回文件与指定模型的相似度

	返回值：
	0 表示成功
	*/
	int CalculateFileSimilarityInModel4(map<string, float> Model, map<string, float> ctWordsList, float& Similarity);

	/*
	描述：
	计算文件中每个词组出现的概率， 词组在文件中重复次数越多概率越高

	参数：
	Words 文件中的所有词汇，词汇课重复
	ctWordsList 返回文件中每个词汇出现的概率
	*/
	int CalculateWordProInFile4(StrVec& Words, map<string, float>& ctWordsList);

	/*
	描述：
	设置一批文件应该属于哪个类

	参数：
	vclass vclass[n]表示第n类中文件的序号

	返回值：
	0 成功
	*/

	int BYSetClassList4(vector< vector<int> > vclass);


	/*
	描述：
	计算词组在一类文件中的概率  词组出现次数/一类文件的词汇总数

	参数：
	content 内容矩阵
	ClassId 指定类的id
	wordPro 返回词汇的概率

	返回值：
	0 成功
	*/
	int BYCreateWordProbabilityInClass4(ContentMatrix& content, int ClassId, map<string, float>& wordPro);

	/*
	描述：
	计算在同一类文件生成的模型中， 每个文件的相似度，返回一个最大值和一个最小值

	参数：
	DatawordMatrix 同一类文件的词组矩阵
	ClassId 模型id
	wordPro 同一类文件词组在指定模型中的词组概率
	MinSimilarity 返回最低相似度
	MaxSimilarity 返回最高相似度

	返回值：
	0 表示成功
	*/
	int CalculateSimilaritryRange4(vector< StrVec > DatawordMatrix, 
		int ClassId, 
		map<string, float>& wordPro,
		float& MinSimilarity,
		float& MaxSimilarity
		);

	/*
	描述：
	计算一个文件属于某个模型的概率，算法是取文件词组与模型词组的交集， 交集词组对应的概率求和

	参数：
	ModelId 模型数组id
	Words 文件含有的词汇表
	Probability 返回文件属于指定模型的概率
	KeyWords 返回文件与模型的交集

	返回值：
	0 表示成功

	*/
	int CalculateFileProInModel4(int ModelId, StrVec Words, float& Probability, vector<string>& KeyWords);

	/*
	描述：
	判断模型是否加载成功
	
	参数：
	无

	返回值：
	true 加载成功
	false 加载失败
	*/
	bool IsModelOk();

   void SetClassName(vector<string>& className);
};
