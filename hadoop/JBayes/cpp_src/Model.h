#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#ifdef WIN32
#define DLL_EXPORT_API __declspec(dllexport)
#else
#define DLL_EXPORT_API
#endif

/*
贝叶斯模型，用来描述一个分类
*/
class DLL_EXPORT_API CModel
{
public:
	string m_level;//安全级别
	string m_modulename;//模型的名字
	string m_uid;
	map<string, float> m_wordPro;//单词的先验概率
	float m_thresholdRange[2];//建议阈值范围
	float m_threshold;//使用阈值
	int m_filenumber;//生成模型所使用的文件数目
   bool m_positive;

public:
	CModel();
	virtual ~CModel();
	virtual int loadmodule();
	virtual int loadmodule(string& path);
	virtual int savemodule();

	map<string, float> *getwordpro();
	string getmodulename();
	string getuid();
	string getlevel();
	int getthresholdrange(float* buffer);
	float getthreshold();
	int getfilenumber();

	int setwordpro(map<string, float>& wordpro);
	int setmodulename (string& modulename);
	int setthresholdrange(float* buffer);
	int setthreshold(float threshold);
	int setfilenumber(int filenumber);

    bool is_positive();
    void set_positive(bool positive);
};
