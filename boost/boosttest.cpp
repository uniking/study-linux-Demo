#include <boost/filesystem.hpp>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
/*
	//base name
	boost::filesystem::path test(".file/tmp/abc.txt");
	std::string mybasename = boost::filesystem::basename(test);
	printf("basename=%s\n", mybasename.c_str());
*/

/*
	//文件层次
	boost::filesystem::path testdir("/home/sxis/workspace/boost/");
	boost::filesystem::path::iterator pathdir=testdir.begin();
	while(pathdir != testdir.end())
	{
		std::cout << *pathdir <<" ";
		pathdir++;
	}
*/

/*
	//枚举目录文件,第归
	boost::filesystem::path testdir("/home/sxis/workspace/boost/");
	boost::filesystem::recursive_directory_iterator end_iter;
	boost::filesystem::recursive_directory_iterator iter(testdir);
	while(iter != end_iter)
	{
		if(boost::filesystem::is_directory(*iter))
			cout << "one dir file:" << *iter << endl;
		else if(boost::filesystem::extension(*iter) == ".cpp")
			cout << "one .cpp file:" << *iter << endl;
		else
			cout << "other file:" << *iter << endl;
		iter++;
	}
*/


	//枚举目录文件,非第归
	boost::filesystem::path testdir("/home/sxis/workspace/boost/");
	boost::filesystem::directory_iterator end_iter;
	boost::filesystem::directory_iterator iter(testdir);
	while(iter != end_iter)
	{
		if(boost::filesystem::is_directory(*iter))
			cout << "one dir file:" << *iter << endl;
		else if(boost::filesystem::extension(*iter) == ".cpp")
			cout << "one .cpp file:" << *iter << endl;
		else
		{
			string otherfile = iter->path().string();
			cout << "other file:" << otherfile << endl;
		}
		iter++;
	}


/*
	//目录拼接
	boost::filesystem::path testdir("/home/sxis/workspace/boost/");
	cout<<testdir<<endl;

	//testdir+="abcd";
	boost::filesystem::path test1("abcd");
	testdir += test1;
	cout<<testdir<<endl;

	//testdir+="wang";
	boost::filesystem::path test2("wang");
	testdir += test2;
	cout<<testdir<<endl;

	testdir /= "hao/";
	cout<<testdir<<endl;

	testdir /= "dao";
	cout<<testdir<<endl;
*/

	return 0;
}
