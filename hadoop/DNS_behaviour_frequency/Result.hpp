#ifndef _RESULT_H
#define _RESULT_H

#include <string>
#include <iostream>
#include <map>
using namespace std;

class CHostResult
{
public:
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
	map<string, CHostResult> m_rsult;//<time, result>

	CResult();
	void info();
};
#endif //_RESULT_H
