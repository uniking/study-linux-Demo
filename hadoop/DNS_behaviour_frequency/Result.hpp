#ifndef _RESULT_H
#define _RESULT_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class CHostResult
{
public:
	string m_hostname;
	string m_time;
	bool m_anomie;
	bool m_inmodel;

	CHostResult();
	CHostResult(bool anomie);
};

class CResult
{
public:
	string m_user;
	bool m_anomie;
	bool m_inmodel;
	float m_similarity;

	vector<CHostResult> m_result;//<time, result>

	CResult();
	void info();
};
#endif //_RESULT_H
