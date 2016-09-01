#ifndef _RESULT_H
#define _RESULT_H

#include <string>
#include <iostream>
using namespace std;

class CResult
{
public:
	string m_user;
	bool m_anomie;
	bool m_inmodel;
	float m_similarity;

	CResult();
	void info();
};
#endif //_RESULT_H
