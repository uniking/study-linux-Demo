#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <vector>
#include <map>

using namespace std;


map<string,char> whiteModule;
map<string, string> appMap;//path,exe

bool startWith(const string& content, const string& prefix)
{
	int i=0;
	int size = prefix.size();
	for(; i<size; ++i)	
	{
		if(content[i] != prefix[i])
			break;
	}

	if(i==size)
		return true;
	return false;
}

bool detectModule(map<string, string> am, map<string, char> whiteM, vector<string>& result)
{
	bool bRtn = false;
	map<string, string>::iterator iam = am.begin();
	while(iam != am.end())
	{
		if(whiteM.find(iam->first) == whiteM.end())
		{//not find, find sys lib
		// /system/lib/*
		// /system/bin/*
		// /system/vendor/lib/*
		// /data/dalvik-cache/arm/*
		// [vectors]
			if(startWith(iam->first, "/system/lib/") ||
				startWith(iam->first, "/system/bin/") ||
				startWith(iam->first, "/system/vendor/lib/") ||
				startWith(iam->first, "[vectors]")
			)
			{
				cout<<"insert white list:"<<iam->first<<endl;
				whiteM.insert(make_pair(iam->first, 1));
			}
			else
			{
				result.push_back(iam->first);
				bRtn = true;
			}
		}
		else
		{
		}
		++iam;
	}

	return bRtn;
}

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
        if(token.size() != 0)
            tokens.push_back(token);
   }
   return tokens;
}

void splitString(string msg)
{
	vector<string> lines = split(msg, '\n');
	vector<string>::iterator il = lines.begin();
	int i=0;
	while(il != lines.end())
	{
		//cout<<i<<":"<<*il<<endl;
		vector<string> words = split(*il, ' ');
		/*
		vector<string>::iterator iw = words.begin();
		int j=0;
		while(iw != words.end())
		{
			if(iw->size() != 0)
			  cout<<"words "<<j<<":"<<*iw<<endl;

			++iw;
			++j;
		}
		*/
		if(words.size() == 6)
		{
		    //cout<<words[5]<<endl;
		    if(words[1][2] == 'x')//have exec permission
		        appMap.insert(make_pair(words[5], words[1]));
		}

		++i;
		++il;
	}
}

int main()
{
	string mapP = "/proc/";
	pid_t pid = getpid();
	std::ostringstream oss;
	oss << pid;
	mapP += oss.str();
	mapP += "/maps";
	//cout<<mapP<<endl;

	int fd = open(mapP.c_str(), O_RDONLY);
	if(fd == -1)
	{
		cout<<"open map err:"<< strerror(errno)<<endl;
		return -1;
	}

	string content;
	char buf[1025];
	ssize_t size = 0;
	while((size = read(fd, buf, 1024)) != -1)
	{
		if(size == 0)
			break;
		//cout<<"read size="<<size<<endl;
		buf[size] = 0;
		content += buf;
	}

	//cout<<content<<endl;

	splitString(content);


	map<string, string>::iterator im = appMap.begin();
	while(im != appMap.end())
	{
		if(im->second[2] == 'x')
		cout<<im->first<<" "<<im->second<<endl;
		++im;
	}


	vector<string> result;
	if(detectModule(appMap, whiteModule, result))
		cout<<"find threats!"<<endl;
	

	close(fd);
}

