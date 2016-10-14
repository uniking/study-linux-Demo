#pragma once

#include <iostream>
#include <string>
#include<fstream>
#include <sstream> 
#include <iosfwd>
#include <vector>
using namespace std;

/*
方便linux，windows下枚举目录的功能， 
发现boost有两个平台的目录枚举功能， 最好替换为boost的
*/

#ifdef WIN32
#define DLL_EXPORT_API __declspec(dllexport)
#else
#define DLL_EXPORT_API
#endif

namespace discover
{

typedef bool (*EnumerateFunc2) (char* lpFileOrPath, vector<string>& FileList);
class DLL_EXPORT_API File
{
public:
	virtual ~File() {};

	
	virtual int ReadDirFile2(char* cSrcDir, vector<string>& dataMatrix , vector<string>& vsFileList);
	virtual void doFileEnumeration2 (string lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList);

protected:
	File() {};
};


#ifdef _WINDOWS
#include <Windows.h>


class DLL_EXPORT_API winFile:public File
{
public:
	winFile(){};
	virtual ~winFile(){};

	int ReadDirFile2(char* cSrcDir, vector<string>& dataMatrix , vector<string>& vsFileList);
	void doFileEnumeration2(string lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList);
	BOOL StringToWString(const std::string &str,std::wstring &wstr);
	BOOL WStringToString(const std::wstring &wstr,std::string &str);
	void cutFileName(vector<string> &FilePath, vector<string> &FileName);
};
#else
class linuxFile:public File
{
public:
	linuxFile(){};
	virtual ~linuxFile(){};
	int ReadDirFile2 (char* cSrcDir, vector<string>& dataMatrix , vector<string>& vsFileList);
	//void doFileEnumeration2(string lpPath, bool bRecursion, bool bEnumFiles, char* pFunc, vector<string>& FileList);
	void doFileEnumeration2(string lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList);
};

}
#endif
