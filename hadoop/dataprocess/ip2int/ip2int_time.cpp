#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

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

long ip_to_long(string& ip_addr)
{
	vector<string> line_words;
	Tokenize(ip_addr, line_words, " ");
	cout<<line_words[0]<<" ";

	vector<string>::iterator p_word = line_words.begin();
	p_word++;
	bool bFirst = true;
	while(p_word != line_words.end())
	{
		//cout<<*p_word<<endl;
		vector<string> ip_part;
		Tokenize(*p_word, ip_part, ".");
		vector<string>::iterator p_ip_part = ip_part.begin();

		unsigned char ip[4] = {0};
		int index = 0;
		while(p_ip_part != ip_part.end())
		{
			//cout<<*p_ip_part<<endl;
			ip[index] = (unsigned char)atoi((*p_ip_part).c_str());
			p_ip_part++;
			index++;
		}

		unsigned long *p_ip = (unsigned long *)ip;
		cout<<*p_ip;

		if(bFirst)
		{
			bFirst = false;
			cout<<" ";
		}
		p_word++;
	}

	return 0;
}

int main(int argn, char* argv[])
{
	ifstream file(argv[1]);
	if(!file.is_open())
		return -1;

	
	while(true)
	{
		string sub;
		getline(file, sub);
		if(sub.size() == 0)
			break;

		ip_to_long(sub);
		cout<<endl;
	}




	return 0;	
}
