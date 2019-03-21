#include "FileSystem.h"

namespace discover
{

int File::ReadDirFile2(char* cSrcDir, vector<string>& dataMatrix , vector<string>& vsFileList)
{
	return 0;
}

void File::doFileEnumeration2(string lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList)
{

}

#ifdef _WINDOWS
void winFile::doFileEnumeration2(string strPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList)  
{  

	static BOOL s_bUserBreak = FALSE;
	const char* lpPath = strPath.c_str();
	try{  
		//-------------------------------------------------------------------------  
		if(s_bUserBreak) return;  

		int len = strlen(lpPath);  
		if(lpPath==NULL || len<=0) return;  

		//NotifySys(NRS_DO_EVENTS, 0,0);  

		char path[MAX_PATH];  
		//wcscpy(path, (wchar_t *)lpPath); 
		strncpy(path, lpPath, MAX_PATH);  
		if(lpPath[len-1] != '//')  strcat(path, "//");  
		strcat(path, "*");  

		WIN32_FIND_DATAA fd;  
		HANDLE hFindFile = FindFirstFileA(path, (LPWIN32_FIND_DATAA)&fd);  
		if(hFindFile == INVALID_HANDLE_VALUE)  
		{  
			::FindClose(hFindFile); return;  
		}  

		char tempPath[MAX_PATH] = {0}; 
		BOOL bUserReture=TRUE; 
		BOOL bIsDirectory;  

		BOOL bFinish = FALSE;  
		while(!bFinish)  
		{  
			strncpy(tempPath, lpPath, MAX_PATH);  
			if(lpPath[len-1] != '\\') strcat(tempPath, "\\");  
			strcat(tempPath, fd.cFileName);  

			bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  

			//如果是.或..  
			if( bIsDirectory  
				&& (strcmp((const char *)fd.cFileName, ".")==0 || strcmp((const char *)fd.cFileName, "..")==0))   
			{         
				bFinish = (FindNextFileA(hFindFile, &fd) == FALSE);  
				continue;  
			}  

			if(bEnumFiles!=bIsDirectory)  
			{  
				if(pFunc)
				{
					pFunc(tempPath, FileList);
				}
				else
				{
					string strFileName(tempPath);
					FileList.push_back(strFileName);
				}

				
			}  

			//NotifySys(NRS_DO_EVENTS, 0,0);  

			if(bIsDirectory && bRecursion) //是子目录  
			{  
				doFileEnumeration2(string(tempPath), bRecursion, bEnumFiles, pFunc, FileList);  
			}  

			bFinish = (FindNextFileA(hFindFile, &fd) == FALSE);  
		}  

		::FindClose(hFindFile);  

		//-------------------------------------------------------------------------  
	}catch(...){ /*ASSERT(0); */return; }  
} 

bool myEnumerateFunc(char* lpFileOrPath, vector<string>& FileList)  
{  
	FileList.insert(FileList.begin(), lpFileOrPath);
	return TRUE;  
} 


BOOL winFile::StringToWString(const std::string &str,std::wstring &wstr)
{    
	int nLen = (int)str.length();    
	wstr.resize(nLen,L' ');

	int nResult = MultiByteToWideChar(CP_ACP,0,(LPCSTR)str.c_str(),nLen,(LPWSTR)wstr.c_str(),nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}
//wstring高字节不为0，返回FALSE
BOOL winFile::WStringToString(const std::wstring &wstr,std::string &str)
{    
	int nLen = (int)wstr.length();    
	str.resize(nLen,' ');

	int nResult = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wstr.c_str(),nLen,(LPSTR)str.c_str(),nLen,NULL,NULL);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}

void winFile::cutFileName(vector<string> &FilePath, vector<string> &FileName)
{
	vector<string>::iterator itFilePath = FilePath.begin();
	string temp;

	for(;itFilePath != FilePath.end(); ++itFilePath)
	{
		temp = *itFilePath;
		string::size_type i = temp.rfind("\\");
		if (string::npos == i)
		{
			FileName.push_back(temp);
			continue;
		}
		++i;
		string ssubTmp = temp.substr(i);
		FileName.push_back(ssubTmp);
	}
}

//读取目录文件，返回数据
int winFile::ReadDirFile2(char* cSrcDir, vector<string>& dataMatrix, vector<string>& vsFileList)
{
	int i=0;
	vector<string> FileList;

	doFileEnumeration2(cSrcDir, false, true,myEnumerateFunc,FileList);

	vector<string> vFileNameList;
	cutFileName(FileList, vFileNameList);

	vsFileList = vFileNameList;

	vector<string> strVec;

	vector<string>::iterator itFile;
	for(i=0, itFile=FileList.begin(); itFile != FileList.end(); ++itFile,++i)
	{
		string sFileName = *itFile;
		wstring wsFileName;
		char* FileBuffer = (char*)sFileName.c_str();
		//cout<<i<<"  "<<sFileName.c_str()<<endl;
		StringToWString(*itFile, wsFileName);

		ifstream inFile(wsFileName.c_str()); 
		ostringstream os;
		os << inFile.rdbuf();

		strVec.push_back(os.str());
	}

	dataMatrix = strVec;
	return 0;
}

#else
//------------------------------linux
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>
using namespace std;
int linuxFile::ReadDirFile2(char* cSrcDir, vector<string>& dataMatrix , vector<string>& vsFileList)
{
	return 0;
}

void linuxFile::doFileEnumeration2(string lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc2 pFunc, vector<string>& FileList)
{

	string rootPath;
	struct stat statbuf;
	char buffer[1024];


	DIR *dp = opendir(lpPath.c_str());
	if(dp == NULL)
		return;

	if((lpPath.c_str())[0] != '/')
	{
		rootPath = getcwd(buffer, 1024);
		strncpy(buffer, &(lpPath.c_str())[1], 1024);
		rootPath += buffer;
	}
	else
	{
		rootPath = lpPath;
	}

	struct dirent *pD = NULL;

	while((pD = readdir(dp)) != NULL)
	{
		if(strcmp(".", pD->d_name) == 0 ||
			strcmp("..", pD->d_name) == 0)
			continue;

		string FilePath;
		FilePath = rootPath+"/"+pD->d_name;

		//lstat(pD->d_name, &statbuf);
		lstat(FilePath.c_str(), &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{//目录
			if(bRecursion == true)
			{//递归遍历
			}
		}
		else
		{//文件
			if(pFunc)
			{
				pFunc((char*)FilePath.c_str(), FileList);
			}
			else
			{
				FileList.push_back(FilePath);
			}
			
			//cout<<FilePath<<endl;
		}

	}


	closedir(dp);
}

#endif

}


