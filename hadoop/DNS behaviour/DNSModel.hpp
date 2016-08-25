#ifndef _DNS_MODEL_H
#define _DNS_MODEL_H

#include "DataItem.hpp"
#include "Result.hpp"

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
using namespace std;



class CDNSModel
{
private:
	string m_site;
	string m_user;
	list<DATA_ITEM> m_Matrix;
	int m_size;
	DATA_ITEM m_center;
	float m_max_similarity;
	float m_min_similarity;
	float m_mean_similarity;
	float m_sum_similarity;

	float similarity(DATA_ITEM& first, DATA_ITEM& second);
	void statistics(list<DATA_ITEM>& Matrix);


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
