#include "DNSModel.hpp"

CDNSModel::CDNSModel()
{
	m_site = "";
	m_user = "";
	m_max_similarity = 0;
	m_min_similarity = 1;
	m_mean_similarity = 0;
	m_sum_similarity = 0;
}

float CDNSModel::similarity(DATA_ITEM& first, DATA_ITEM& second)
{
	if(first.hostname.size() == 0 || second.hostname.size() == 0)
		return 0;

	float sum = first.hostname.size() + second.hostname.size();
	float same_sum=0;
	vector<string>::iterator pF = first.hostname.begin();
	while(pF != first.hostname.end())
	{
		vector<string>::iterator pS = second.hostname.begin();
		while(pS != second.hostname.end())
		{
			if(*pF == *pS)
			{
				same_sum++;
				break;
			}
			pS++;
		}

		pF++;
	}

	return same_sum*2/sum;
}


void CDNSModel::statistics(list<DATA_ITEM>& Matrix)
{
	float max_similarity = 0;
	float min_similarity = 1;
	float sum_similarity = 0;

	m_sum_similarity = 0;
	m_size = Matrix.size();

	list<DATA_ITEM>::iterator item = Matrix.begin();
	while(item != Matrix.end())
	{
		list<DATA_ITEM>::iterator itemC = Matrix.begin();
		while(itemC != Matrix.end())
		{
			float tmp = similarity(*item, *itemC);
			sum_similarity += tmp;
			if(tmp > max_similarity)
				max_similarity = tmp;
			if(tmp < min_similarity)
				min_similarity = tmp;

			itemC++;
		}

		if(sum_similarity > m_sum_similarity)
		{
			m_center = *item;

			m_max_similarity = max_similarity;
			m_min_similarity = min_similarity;
			m_sum_similarity = sum_similarity;
		}

		max_similarity = 0;
		min_similarity = 1;
		sum_similarity = 0;

		item++;
	}

}

bool CDNSModel::generate(string site, string user, list<DATA_ITEM>& Matrix)
{
	m_site = site;
	m_user = user;
	statistics(Matrix);
}

CResult CDNSModel::anomie(DATA_ITEM& item)
{
	CResult Rtn;
	float si = similarity(item, m_center);
	
	if(si >= m_min_similarity)
		Rtn.m_anomie = false;
	else
		Rtn.m_anomie = true;

	Rtn.m_similarity = si;
	Rtn.m_user = item.user;

	return Rtn;
}

bool CDNSModel::save_model(const string& path)
{
	return false;
}

bool CDNSModel::load_model(const string& path)
{
	return false;
}

void CDNSModel::info()
{	
	cout<<"size:"<<m_size<<endl;
	cout<<"max_similarity:"<<m_max_similarity<<endl;
	cout<<"min_similarity:"<<m_min_similarity<<endl;
	cout<<"center item:"<<endl;
	m_center.info();
}


