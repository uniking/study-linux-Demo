#include "Result.hpp"

extern bool new_user_default_anomie;

CResult::CResult()
{
	m_user = "";
	m_anomie = new_user_default_anomie;
	m_inmodel = true;
	m_similarity = 0;
}

void CResult::info()
{
	cout<<"result:"<<" user:"<<m_user<<" inmodel:"<<m_inmodel<<" anomie:"<<m_anomie<<" similarity:"<<m_similarity<<endl;
}
