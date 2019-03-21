#pragma once
#ifdef WIN32
#include <Windows.h>
#endif

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
using namespace std;



#define  ContentMatrix vector<vector<float> >
#define  TFIDFMatrix vector<map<float,float> > //(id,概率)
typedef vector<string> StrVec; //字符串向量
typedef vector<int> IntVec; //整数向量
typedef vector<vector<int> > Int2DVec;//整数二维向量
typedef vector<vector<double> > Double2DVec;//浮点数二维向量
typedef vector<double> DoubleVec;//浮点数向量

void ictclas30(string& input, StrVec& retWords);

void TFIDF(ContentMatrix& content, TFIDFMatrix& tfidf);
void TFIDFKMeans(TFIDFMatrix& tfidf, int k, vector<int>& vclass, TFIDFMatrix& centAttri);
void MapToMultimap(map<float, float>& fMap, multimap<float, float>& mMap);

void ReverseSelectAttrFromMultimap(multimap<float, float>& mMap, vector<float>& Attr , int n);
void ReverseSelectAttrFromMultimap2(multimap<float, float>& mMap, vector<float>& Attr);
/*
content 内容矩阵
tfidf TFIDF运算结果，包含了文章总所有词的TFIDF值
vclass 指定tfidf中的分类情况
classatrribute 返回抽取的类属性
*/
int BYSelectAttributeFromClassFile(TFIDFMatrix tfidf, 
								   vector< vector<int> > vclass, 
								   vector< vector<float> >& classattribute);

/*
ClassNumber 要设置的类id
classatrribute 属性列表，这个列表用来生成类模型
*/
int BYSetClassAttribute(int ClassId, vector<float> classatrribute);


/*
判断内容所属类的
Content 要判断的文本内容
ProbabilityList 返回在各个类中的概率
*/
int BYPredict(string Content, vector<float>& ProbabilityList);

/*
为判断是否为敏感文件设置阈值
threshold 阈值，所属类的概率超过此阈值将视为敏感文件
*/
int SetThreshold(float threshold);

/*
判断是否是敏感信息，需要预先设置阈值
Content 要判断的文本内容
ClassName 返回所属的类名
如果返回false，则说明不属于任何类
*/
bool IsAttractiveContent(string Content, string& ClassName, float& oSimilarity, vector<string>& KeyWords);

int GetModelNameById(int ClassId, string& ClassName);

int GetWordById(float id, string& word);
int GetIdByWord(string& word, float& id);
int InsertWordAndId(string word, float id);
bool TFWordIdOk();
