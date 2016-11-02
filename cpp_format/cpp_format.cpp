#include "FileSystem.hpp"
#include <string.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void delete_enter_tail(string& content)
{
	int point=0;
	while(point < content.size())
	{
		point = content.find('\n', point);
		if(point == content.npos || point >= content.size())
			break;
		if(content[point+1]==' ' || content[point+1]=='\t')
			content.erase(point+1, 1);
		else
			point += 1;
	}
}

void delete_enter_head(string& content)
{
	int point=0;
	while(point < content.size())
	{
		point = content.find('\n', point);
		if(point == content.npos || point >= content.size() || point <= 0)
			break;
		if(content[point-1]==' ' || content[point-1]=='\t')
		{
			content.erase(point-1, 1);
			point -= 1;
		}
		else
			point += 1;
	}
}

void delete_enter_between_enter(string& content)
{
	int point=0;
	int point2=0;
	while(point < content.size())
	{
		point = content.find('\n', point);
		if(point == content.npos)
			break;
		point2 = content.find('\n', point+1);
		if(point == content.npos)
			break;

		if(point2 - point == 1)
		{// /n/n
			content.erase(point2, 1);
		}
		else
		{
			cout<<point<<" "<<point2<<endl;
			int index = point;
			bool del = true;
			while(index < point2)
			{// only ' ' or '\t' 
				if(content[index] == ' ' || content[index] == '\t')
					index++;
				else
				{
					del = false;
					break;
				}
			}
			if(del)
			{
				content.erase(point+1, point2-point);
			}
		}

		point = point2-1;
		
	}
}

void delete_file_head(string& content)
{
	while(content[0] == ' ' || content[0] == '\t')
	{
		if(content.size() == 0)
			break;
		content.erase(0, 1);
	}
}

void add_enter_left_brace(string& content)
{
	int point = 0;
	while(point < content.size())
	{
		point = content.find('{', point);
		if(point == content.npos)
			return;
		content.insert(point+1, "\n");
		content.insert(point, "\n");
		point += 2;
	}
}

void add_enter_right_brace(string& content)
{
	int point = 0;
	while(point < content.size())
	{
		point = content.find('}', point);
		if(point == content.npos)
			return;
		content.insert(point+1, "\n");
		content.insert(point, "\n");
		point += 2;
	}
}

void add_enter_right_semicolon(string& content)
{
	int point = 0;
	while(point < content.size())
	{
		point = content.find(';', point);
		if(point == content.npos)
			return;
		content.insert(point+1, "\n");
		point += 2;
	}
}

// utf8 or linux ansi
void format(string filepath)
{
// 0, delete ' '/'\t'/'\n' after /n     =>delete null line and delete space/tab in head line
// 1, /n + { + /n
// 2, /n + } + /n
// 3, ; + /n
//  => code have not tail space but have null line

// 4, delete null line

// 5, delete space/tab in head line
// 6, delete space/tab int tail line

// 7, push pop to { add tab/4space



/*
		add(12, 34, \
		56, 78);
*/



	ifstream infile2(filepath.c_str());
	if(!infile2.is_open())
	{
		cout<<filepath<<" open error"<<endl;
		return;
	}

	string content;
	ostringstream os;
	os << infile2.rdbuf();
	content = os.str();

	cout<<content<<endl<<endl;
	int point2=0;
	int point = 0;

	

	//
	delete_file_head(content);

	//0
	delete_enter_tail(content);
	delete_enter_head(content);

	//1
	add_enter_left_brace(content);

	//2
	add_enter_right_brace(content);

	//2
	add_enter_right_semicolon(content);

	//4
	delete_enter_between_enter(content);

	//6




	delete_enter_tail(content);
	delete_enter_between_enter(content);
	cout<<content<<endl;
	
}

void read_ini(string filepath)
{
}

bool ef(char* lpFileOrPath, vector<string>& FileList)
{
	cout<<lpFileOrPath<<endl;
	int len = strlen(lpFileOrPath);
	if(len > 4 && strncmp(lpFileOrPath+len-4, ".cpp", 4) == 0)
	{
		FileList.push_back(lpFileOrPath);
		return true;
	}
	else
		return false;
}

int main()
{
	read_ini("./format.ini");

	vector<string> FileList;
	discover::linuxFile lf;
	lf.doFileEnumeration2("./", true, true, ef, FileList);

	vector<string>::iterator fp = FileList.begin();
	while(fp != FileList.end())
	{
		format(*fp);
		fp++;
	}

}
