#include "Result.hpp"

extern bool new_user_default_anomie;

CHostResult::CHostResult()
{
}

CHostResult::CHostResult(bool anomie)
{
	m_anomie = anomie;
}

CResult::CResult()
{
	m_user = "";
	m_anomie = new_user_default_anomie;
	m_inmodel = true;
}

void CResult::info()
{
	cout<<"result:"<<" user:"<<m_user<<" inmodel:"<<m_inmodel<<" anomie:"<<m_anomie<<endl;
}
