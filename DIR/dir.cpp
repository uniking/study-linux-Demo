#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>
using namespace std;

void doFileEnumeration(string lpPath, bool bRecursion, bool bEnumFiles, char* pFunc, vector<string>& FileList)
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

		lstat(pD->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{//目录
			if(bRecursion == true)
			{//递归遍历
			}
		}
		else
		{//文件
			string FilePath;
			FilePath = rootPath+"/"+pD->d_name;
			FileList.push_back(FilePath);
			printf("%s\n", FilePath.c_str());
		}
		
	}


	closedir(dp);

}

int main()
{
	string path("./");
	vector<string> filelist;
	char cwd[1024] = {0};
	char* p;
	p = getcwd(cwd, 1024);
	printf("%s  %s\n", p, cwd);

	mkdir("./test", 0775);

	//doFileEnumeration(path, false, true, NULL, filelist);
	return 0;
}
