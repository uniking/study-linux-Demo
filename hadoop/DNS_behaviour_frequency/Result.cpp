#include "Result.hpp"

extern bool new_user_default_anomie;

CHostResult::CHostResult()
{
	m_anomie = new_user_default_anomie;
	m_inmodel = false;
}

CHostResult::CHostResult(bool anomie)
{
	m_anomie = anomie;
}

CResult::CResult()
{
	m_user = "";
}

void CResult::info()
{
	//cout<<"result:"<<" user:"<<m_user<<" inmodel:"<<m_inmodel<<" anomie:"<<m_anomie<<endl;

	cout<<"result: user:"<<m_user<<endl;

	if(m_result.size() == 0)
	{
		cout<<"time: null"<<" anomie:"<<m_anomie<<" inmodel:"<<m_inmodel<<endl;
		cout<<endl;
		return;
	}

	vector<CHostResult>::iterator pHT = m_result.begin();
	while(pHT != m_result.end())
	{
		cout<<"time:"<<pHT->m_time<<" anomie:"<<pHT->m_anomie<<" inmodel:"<<pHT->m_inmodel<<" hostname:"<<pHT->m_hostname<<endl;
		pHT++;
	}

	cout<<endl;
}
