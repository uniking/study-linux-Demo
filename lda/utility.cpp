#include "utility.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>

namespace discover
{
	int JsonWrite(const string& path, Json::Value& root_json)
	{
		ofstream outfile(path.c_str());

		Json::FastWriter fast_writer;
		string temp = fast_writer.write(root_json);
		outfile << temp;

		return 0;
	}

	int JsonRead(const string& path, Json::Value& root_json)
	{
		ifstream infile(path.c_str());

		if(!infile.is_open())
			return -1;

		string sContent;
		ostringstream os;
		os << infile.rdbuf();
		sContent = os.str();

		Json::Reader reader(Json::Features::strictMode());

		if (!reader.parse(sContent.c_str(), root_json))
		{
			return 1;
		}

		return 0;
	}

	float atof(string f_num)
	{
		int integer=0;
		int decimal=0;
		int dec_len = 0;

		stringstream ss(f_num);
		string sub_str;
		if(getline(ss,sub_str,'.'))
			integer = atoi(sub_str.c_str());

		if(getline(ss,sub_str,'.'))
		{
			dec_len=sub_str.size();
			decimal = atoi(sub_str.c_str());
		}

		float f_ret=decimal;
		f_ret /= pow(10.0, dec_len);
		f_ret += integer;
		return f_ret;
	}

}//namespace discover
