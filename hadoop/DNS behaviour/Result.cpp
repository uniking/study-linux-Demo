#include "Result.hpp"

CResult::CResult()
{
	m_user = "";
	m_anomie = true;
	m_inmodel = true;
	m_similarity = 0;
}

void CResult::info()
{
	cout<<"result:"<<" user:"<<m_user<<" inmodel:"<<m_inmodel<<" anomie:"<<m_anomie<<" similarity:"<<m_similarity<<endl;
}
