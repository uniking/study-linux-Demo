#include "FileModel.h"
#include <json/json.h>
#include <stdlib.h>

#ifdef _WIN32
#include "winiconv.h"
#endif

CFileModel::CFileModel(string& path)
{
	m_filepath = path;
}

CFileModel::CFileModel()
{
}


int CFileModel::JsonRead(string& sContent)
{
	string tmp;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;

#ifdef _WIN32
	sContent = UTF8ToGBK(sContent);
#endif

	if (!reader.parse(sContent.c_str(), json_object))
	{
		return 0;
	}

	//string tmp_str;
	m_level = (json_object["level"]).asString();
	m_modulename = (json_object["modulname"]).asString();
	m_thresholdRange[0] = (json_object["lowThreshold"]).asDouble();
	m_thresholdRange[1] = (json_object["highThreshold"]).asDouble();
	m_threshold = (json_object["threshold"]).asDouble();
   m_filenumber = (json_object["fileNum"]).asInt();
   m_positive = (json_object["positive"]).asInt();
	
	m_uid = (json_object["uid"]).asString();
	
	tmp =  (json_object["wordPro"]).asString();
	istringstream istr(tmp);
	while(!istr.eof())
	{
		string sTmp;
		///inFile.getline(sTmp)
		istr >> sTmp;

		//getline(inFile, sTmp);
		if (sTmp.size() == 0)
		{
			break;
		}

		string::size_type iPosition = sTmp.find (":");
		string sFeaId = sTmp.substr (0, iPosition);

		iPosition += string (":").length();
		string sProb = sTmp.substr (iPosition);

		m_wordPro.insert(make_pair(sFeaId, atof (sProb.c_str())));
	}
	
	return 0;
}
int CFileModel::loadmodule()
{
	//JsonRead(content);
	return -1;
}

int CFileModel::loadmodule(string& content)
{
	JsonRead(content);
	return 0;
}

int CFileModel::JsonWrite()
{
	ofstream outfile(m_filepath.c_str());

	Json::Value root;
	root["modulname"] = Json::Value(m_modulename.c_str());
	root["lowThreshold"] = Json::Value(m_thresholdRange[0]);
	root["highThreshold"] = Json::Value(m_thresholdRange[1]);
	root["threshold"] = Json::Value(m_threshold);
   root["fileNum"] = Json::Value(m_filenumber);
   root["positive"] = Json::Value(m_positive);

	ostringstream os;
	map<string, float>::iterator pWord = m_wordPro.begin();
	while(pWord != m_wordPro.end())
	{
		os<<pWord->first<<":"<< pWord->second << endl;
		++pWord;
	}
	root["wordPro"] = Json::Value(os.str());

	Json::FastWriter fast_writer;
	string temp = fast_writer.write(root);
#ifdef _WIN32
	string utf8str = GBKToUTF8(temp);
	outfile << utf8str;
#else
	outfile << temp;
#endif
	

	return 0;
}

int CFileModel::savemodule()
{
	JsonWrite();
	return 0;
}
