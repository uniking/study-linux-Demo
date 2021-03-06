#ifndef _DATA_ITEM
#define _DATA_ITEM

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class DATA_ITEM
{
public:
	int year;
	int month;
	int day;
	string strTime;

	int e;
	string user;
	string sip;
	vector<string> hostname;
	vector<string> hostname_redundancy;

	float mean;
	float variance;
	float CV;

	void operator = (DATA_ITEM& d)
	{
		year = d.year;
		month = d.month;
		day = d.day;
		strTime = d.strTime;

		e = d.e;
		sip = d.sip;
		user = d.user;
		hostname = d.hostname;
		hostname_redundancy = d.hostname_redundancy;

		mean = d.mean;
		variance = d.variance;
	};

	void info();
};

#endif//_DATA_ITEM
