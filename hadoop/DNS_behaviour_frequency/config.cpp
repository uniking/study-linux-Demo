#include "config.hpp"

int model_shorttest_cycle=5;
bool new_user_default_anomie=false;
bool null_do_normal = true;

#define CONFIG_PATH "./dns_model.ini"

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

int init_config(char* config_path)
{
	string path;

	if(config_path == NULL)
		path=CONFIG_PATH;
	else
		path=config_path;

	Json::Value root_json;
	JsonRead(path, root_json);

	model_shorttest_cycle = root_json["MODEL_SHORTTEST_CYCLE"].asInt();
	new_user_default_anomie = root_json["DEFAULT_ANOMIE"].asBool();
	null_do_normal = root_json["NULL_DO_NORMAL"].asBool();
}
