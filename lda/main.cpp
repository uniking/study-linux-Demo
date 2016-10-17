#include "FileSystem.h"
#include "utility.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

typedef map<int, map<string, float> > model_type;//<top,<word,pro>>

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

bool load_model(char* path, model_type& model)
{
	ifstream ifile(path);
	if(!ifile.is_open())
	{
		printf("open error\n");
		return false;
	}

	int nTop = 0;
	string s;
	while(getline(ifile, s))
	{
		char topic_name[32];
		vector<string> tokens;
		Tokenize(s, tokens, " ");
		if(tokens.size() != 2)
			continue;
		bool bT = tokens[0] == "Topic" ?true:false;
		if(bT)
			sprintf(topic_name, "%dth:", nTop);

		if(bT && tokens[1] == topic_name)
		{//topic
			nTop++;
		}
		else
		{//words
			model_type::iterator pTop = model.find(nTop);
			if(pTop != model.end())
			{//insert words
				pTop->second.insert(make_pair(tokens[0], atof(tokens[1].c_str()) ));
			}
			else
			{
				map<string, float> topic;
				topic.insert(make_pair(tokens[0], atof(tokens[1].c_str())));
				model.insert(make_pair(nTop, topic));
			}
		}
	}
}

float topic_similarity(map<string, float>& topicA, map<string, float>& topicB)
{
	float s = 0;
	map<string, float>::iterator pWord = topicA.begin();
	while(pWord != topicA.end())
	{
		if(topicB.find(pWord->first) != topicB.end())
			s+=pWord->second;
		pWord++;
	}

	return s;
}

float model_similarity(model_type& modelA, model_type& modelB)
{
	int indexA=0;
	float s = 0;
	model_type::iterator pTopic = modelA.begin();
	while(pTopic != modelA.end())
	{
		model_type::iterator pTopicB = modelB.begin();
		float max_sim = 0;
		int indexB=0;
		int temp_index=0;
		while(pTopicB != modelB.end())
		{			
			float sim =topic_similarity(pTopic->second, pTopicB->second);
			if(max_sim < sim)
			{
				max_sim=sim;
				indexB=temp_index;
			}

			temp_index++;
			pTopicB++;
		}

		cout<<indexA<<" "<<indexB<<":"<<max_sim<<endl;
		indexA++;

		pTopic++;
	}

	return s;
}

bool ef(char* lpFileOrPath, vector<string>& FileList)
{
	long fl = strlen(lpFileOrPath);
	string name = "meta";
	char* postfix = lpFileOrPath+fl-4;
	if(name == postfix)
	{
		printf("%s\n", lpFileOrPath);
		FileList.push_back(lpFileOrPath);
		return true;
	}

	return false;
}

int file2words(string& file_path, string& words, string& sip)
{
	char port[12];
	Json::Value root_json;
	discover::JsonRead(file_path, root_json);

	sip = root_json["i_addr"].asString();

	//words += root_json["i_addr"].asString();
	//words += " ";
	//sprintf(port, "%d", root_json["i_port"].asInt());
	//words += port;
	//words += " ";
	words += root_json["o_addr"].asString();
	words += " ";
	sprintf(port, "%d", root_json["o_port"].asInt());
	words += port;
	words += " ";
	words += root_json["s_host"].asString();
	words += " ";
	//words += root_json["file_url"].asString();
	//words += " ";
	words += root_json["mime_type"].asString();
	words += " ";

	words += root_json["protocol"].asString();
}

int collect_test()
{
	char count[16]= {0};
	vector<string> FileList;
	discover::linuxFile lf;
	lf.doFileEnumeration2("/home/sxis/source/2016-10-1112", true, true, ef, FileList);

	ofstream of("test.data");


	vector<string>::iterator fp = FileList.begin();
	map<string, string> ipFile;
	while(fp != FileList.end())
	{
		string fc;
		string sip;
		file2words(*fp, fc, sip);
		if(sip.size() < 7)
		{
			cout<<*fp<<" error "<<sip<<endl;
			fp++;
			continue;
		}

		map<string, string>::iterator ipC = ipFile.find(sip);
		if(ipC != ipFile.end())
		{//attach
			ipC->second+=" ";
			ipC->second+=fc;
		}
		else
		{//insert
			ipFile.insert(make_pair(sip, fc));
		}

		fp++;
	}

	//writefile
	sprintf(count, "%d", ipFile.size());
	of<<count;
	of<<"\n";
	map<string, string>::iterator pIpC = ipFile.begin();
	while(pIpC != ipFile.end())
	{
		of<<pIpC->second;
		of<<"\n";
		pIpC++;
	}

	of.close();

	return 0;
}

bool large_second(string a, string b)
{
	return a > b;
}
void print_top_sim(char* theta)
{
	ifstream infile(theta);

	//read file by line
	if(infile.is_open())
	{
		string s;
		int index = 1;
		while(getline(infile, s))
		{
			vector<string> tokens;
			Tokenize(s, tokens, " ");
			sort(tokens.begin(), tokens.end(), large_second);

			int remain = tokens.size() / 10;
			int count = 0;
			double sum = 0;
			vector<string>::iterator pn = tokens.begin();
			while(pn != tokens.end() && count < remain)
			{
				sum += atof(pn->c_str());
				count++;
				pn++;
			}

			if(sum < 0.3)
				cout<<index<<" "<<sum<<endl;
			index++;
		}
	}
}

int sim_test()
{
	model_type modelA;
	model_type modelB;
	load_model("model-final12.twords", modelA);
	load_model("model-final2.twords", modelB);
	model_similarity(modelB, modelA);
	return 0;
}

int main()
{
	//sim_test();
	//collect_test();

	print_top_sim("test_new.data.theta");
	return 0;
}
