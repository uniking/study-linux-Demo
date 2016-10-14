#ifndef _utility_h

#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <jsoncpp/json/json.h>

using namespace std;

namespace discover
{

	int JsonRead(const string& path, Json::Value& root_json);
	int JsonWrite(const string& path, Json::Value& root_json);

	float atof(string f_num);

}

#endif //_utility_h
