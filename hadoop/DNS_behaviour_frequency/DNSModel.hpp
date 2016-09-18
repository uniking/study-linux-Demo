#ifndef _DNS_MODEL_H
#define _DNS_MODEL_H

#include "DataItem.hpp"
#include "Result.hpp"

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

class CHostnameHav
{
public:
	float min;
	float max;
	float mean;
	float sd;
};



class CDNSModel
{
private:
	string m_site;
	string m_user;
	int m_size;
	int m_min_threshold;
	map<string, CHostnameHav> m_models;

	float similarity(DATA_ITEM& first, DATA_ITEM& second);
	void statistics(list<DATA_ITEM>& Matrix, vector<string>& redundancy);

	float similarity_null_normal(DATA_ITEM& first, DATA_ITEM& second);
	void clear_redundancy(list<DATA_ITEM>& Matrix, vector<string>& redundancy);
	void clear_same_hostname(const vector<string>& constHostname, vector<string>& changeHostname);


public:
	CDNSModel();
	// e and sip of Matrix must be same.
	bool generate(string site, string user, list<DATA_ITEM>& Matrix);
	CResult anomie(DATA_ITEM& item);
	bool save_model(const string& path);
	bool load_model(const string& path);

	void info();
};

#endif //_DNS_MODEL_H
