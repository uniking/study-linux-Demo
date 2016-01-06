#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>

using namespace std;


void CRegularKeyWords(Json::Value json_object)
{
	string tmp_str;
	tmp_str = (json_object["class"]).asString();
	tmp_str = (json_object["level"]).asString();
}

void ReadJson()
{
	ifstream infile("./test.txt");
	string sContent;
	ostringstream os;
	os << infile.rdbuf();
	sContent = os.str();

	Json::Reader reader;
	Json::Value json_object;

	if (!reader.parse(sContent.c_str(), json_object))
	{
		cout<<"parse error\n";
	}

	CRegularKeyWords(json_object);
}

int main()
{
	int i=0;
	while(i<50)
	{
		ReadJson();
		i++;
	}
}
