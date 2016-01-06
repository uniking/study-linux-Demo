#include "/home/sxis/workspace/NDLP50000/discover/Dll/jeson/include/json.h"
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class jsonTest
{
private:
	void usejson(Json::Value json_object);
public:
	void sendjson();
};

void jsonTest::usejson(Json::Value json_object)
{
	int classid = (json_object["classid"]).asInt();//各种类型的id
	printf("passJson %d\n", classid);
}

void jsonTest::sendjson()
{
	ifstream infile("./phone.re");
	string sContent;
	ostringstream os;
	os << infile.rdbuf();
	sContent = os.str();

	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(sContent.c_str(), json_object))
	{
		//垃圾文件;
	}

	usejson(json_object);
}
int main()
{

	jsonTest abc;
	abc.sendjson();
	return 0;
}
