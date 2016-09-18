#ifndef CONFIG_H
#define CONFIG_H
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int JsonRead(const string& path, Json::Value& root_json);

int init_config(char* config_path);

#endif //CONFIG_H
