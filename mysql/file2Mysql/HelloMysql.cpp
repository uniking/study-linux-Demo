#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;


void Tokenize(const string& str,
        vector<string>& tokens,
        const string& delimiters)
{  
    // Skip delimiters at beginning.  
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);  
    // Find first "non-delimiter".  
    string::size_type pos     = str.find_first_of(delimiters, lastPos);  
    while (string::npos != pos || string::npos != lastPos)  
    {  
        // Found a token, add it to the vector.  
        tokens.push_back(str.substr(lastPos, pos - lastPos));  
        // Skip delimiters.  Note the "not_of"  
        lastPos = str.find_first_not_of(delimiters, pos);  
        // Find next "non-delimiter"  
        pos = str.find_first_of(delimiters, lastPos);  
    }  
}

/*
127.0.0.1 admin abcd
testdata t1 (id name)
*/
int main(int argn, char* argv[])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* query;
	int t,r;

	if(argn != 2)
	{
		printf("%s filename\n", argv[0]);
		return 1;
	}

	mysql_init(&mysql);
	
	if(!mysql_real_connect(&mysql, "localhost", "root", "abcd", "DNSdata", 0, NULL, 0) )
	{
		printf("mysql_real_connect error\n");
		return 1;
	}

	ifstream ifile(argv[1]);
	if(ifile.is_open())
	{
		string s;
		while(getline(ifile, s))
		{
			vector<string> tokens;
			Tokenize(s, tokens," ");
			if(tokens.size() != 5)
			{
				cout<<"data format error"<<endl;
				continue;
			}

			char query[1024] = {0};
			string ts=tokens[0] + " ";
			ts += tokens[1];
			snprintf(query, 1024, "insert into dns(time, sip, dip, hostname) value(\'%s\',  \'%s\', \'%s\', \'%s\')", ts.c_str(), tokens[2].c_str(), tokens[3].c_str(), tokens[4].c_str());
			t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
			if(t)
			{
				printf("mysql_real_query error\n");
			}
		}
	}
	
	mysql_close(&mysql);
	return 0;
}
