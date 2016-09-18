#ifndef _DATA_ITEM
#define _DATA_ITEM

#include <string>
#include <vector>
#include <iostream>
#include <map>
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
	map<string, long> hostname;//<hostname, count>
	map<string, map<string, long>> hostname_plot; // <hostname, <time, count> >
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
