#include "/home/sxis/workspace/NDLP50000/discover/Dll/jeson/include/json.h"
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void passJson(Json::Value json_object)
{
	int classid = (json_object["classid"]).asInt();//各种类型的id
	printf("passJson %d\n", classid);
}

int main()
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

	passJson(json_object);

	int classid = (json_object["classid"]).asInt();//各种类型的id
	printf("%d\n", classid);
	return 0;
}
