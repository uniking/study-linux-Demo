#include "json/json.h"
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define FILE_NAME "test.json"

// {"0":{"sun0":10},"1":[{"sun":12},{"sun":11}],"int":100,"string":"test"}

class jsonTest
{
private:
	void usejson(Json::Value json_object);
   void json_value();
   int JsonWrite(Json::Value& root, char* filepath);
public:
	void readjson();
   void savejson();
};

void jsonTest::usejson(Json::Value json_object)
{
	int classid = (json_object["int"]).asInt();//各种类型的id
    string test = (json_object["test"]).asString();//各种类型的id
    cout<<classid<<" "<<test<<endl;

    Json::Value sun0 = json_object["0"];
    int sun0_9 = (sun0["sun0"]).asInt();
    cout<<sun0_9<<endl;

    Json::Value sun_l(Json::arrayValue);
    sun_l = json_object["1"];

    Json::Value::iterator tmp = sun_l.begin();
    while(tmp != sun_l.end())
    {
        int i = (*tmp)["sun"].asInt();
        cout<<i<<endl;
        tmp++;
    }
    
}

void jsonTest::readjson()
{
	ifstream infile(FILE_NAME);
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

int jsonTest::JsonWrite(Json::Value& root, char* filepath)
{
	ofstream outfile(filepath);
	ostringstream os;
	Json::FastWriter fast_writer;
	string temp = fast_writer.write(root);
	outfile << temp;
	
	return 0;
}

void jsonTest::savejson()
{
    Json::Value root;

    root["string"] = Json::Value("test");
    root["int"] = Json::Value(100);
    
    Json::Value sun0;
    sun0["sun0"] = Json::Value(10);

/*
    Json::Value sun1;
    sun1["sun1"] = Json::Value(11);

    Json::Value sun2;
    sun2["sun2"] = Json::Value(12);
*/

/*
    Json::Value sun1[2];
    sun1[0] = Json::Value(11);
    sun1[1] = Json::Value(12);
*/


    Json::Value sun1;
    Json::Value sun2;
    Json::Value sun3(Json::arrayValue);
    sun1["sun"] = Json::Value(12);
    sun2["sun"] = Json::Value(11);
    sun3[0] = Json::Value(sun1);
    sun3[1] = Json::Value(sun2);


    root["0"] = Json::Value(sun0);


    root["1"] = Json::Value(sun3);
    JsonWrite(root, "test.json");
}

int main()
{

	jsonTest abc;
    abc.savejson();
	abc.readjson();   
	return 0;
}
