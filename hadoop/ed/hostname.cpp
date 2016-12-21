#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <set>

using namespace std;

int minValue(int a, int b, int c)
{
	int t = a <= b ? a:b;
	return t <= c ? t:c;
}

int calculateStringDistance(string strA, string strB)
{
	int lenA = (int)strA.length()+1;
	int lenB = (int)strB.length()+1;

	int **c = new int*[lenA];
	for(int i = 0; i < lenA; i++)
		c[i] = new int[lenB];

	for(int i = 0; i < lenA; i++) c[i][0] = i;
	for(int j = 0; j < lenB; j++) c[0][j] = j;
		c[0][0] = 0;
	for(int i = 1; i < lenA; i++)
	{
		for(int j = 1; j < lenB; j++)
		{
			if(strB[j-1] == strA[i-1])
				c[i][j] = c[i-1][j-1];
			else
				c[i][j] = minValue(c[i][j-1], c[i-1][j], c[i-1][j-1]) + 1;
		}
	}

	int ret =  c[lenA-1][lenB-1];

	for(int i = 0; i < lenA; i++)
		delete [] c[i];
	delete []c;

	return ret;
}



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
    //     Skip delimiters.  Note the "not_of"  
        lastPos = str.find_first_not_of(delimiters, pos);  
        // Find next "non-delimiter"  
        pos = str.find_first_of(delimiters, lastPos);  
    }  
}


float similar(string a, string b, float ed)
{
	float mean = a.size() + b.size();

	mean=mean/2;
	float s =  (mean -ed)/mean;

	return s;
}

void psd(set<string> &data)
{
	set<string>::iterator pd = data.begin();
	while(pd != data.end())
	{
		set<string>::iterator pds = pd;
		pds++;
		while(pds != data.end())
		{
			if(pd->size() == pds->size() &&
				pd->size() >=25)
			{
				float ed = calculateStringDistance(*pd, *pds);
				float s = similar(*pd, *pds, ed);
				if(s > 0.4)
					cout<<"l:"<<pd->size()<<" d:"<<ed<<" s:"<<s<<"h:"<<*pd<<" "<<*pds<<endl;
			}
			pds++;
		}

		pd++;
	}
}

void ptd(map<string, set<string> > &data)
{
	map<string, set<string> >::iterator pd = data.begin();
	while(pd != data.end())
	{
		cout<<pd->first<<endl;
		if(pd->second.size() >= 2)
			psd(pd->second);
		pd++;
	}
}

int main()
{
	map<string, set<string> > data;

	ifstream infile("oneweb.txt");
	if(!infile)
	{
		cout<<"not exit 1.txt\n";
		return 0;
	}
	else
		cout<<"1 ok\n";

	//read file by line
	if(infile.is_open())
	{
		string s;
		while(getline(infile, s))
		{
			vector<string> sp;
			Tokenize(s,sp,"	");

			map<string, set<string> >::iterator ph = data.find(sp[0]);
			if(ph == data.end())
			{
				set<string> tmp;
				tmp.insert(sp[1]);
				data.insert(make_pair(sp[0], tmp));
			}
			else
			{
				ph->second.insert(sp[1]);
			}
		}

		//calcu s
		ptd(data);
	}


	return 0;
}
