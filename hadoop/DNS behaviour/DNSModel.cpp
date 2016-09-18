#include "DNSModel.hpp"

extern bool null_do_normal;

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

float CDNSModel::similarity_null_normal(DATA_ITEM& first, DATA_ITEM& second)
{
	int sum;
	int dif;

	if(first.hostname.size() >= second.hostname.size())
	{
		sum = first.hostname.size() * 2;
		dif = first.hostname.size() - second.hostname.size();
	}
	else
	{
		sum = second.hostname.size() * 2;
		dif = second.hostname.size() - first.hostname.size();
	}

	if(sum == 0)
		return 1.0;

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

	return (same_sum * 2 + dif)/sum;
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
			float tmp = 0;
			if(null_do_normal)
				tmp = similarity_null_normal(*item, *itemC);
			else
				tmp = similarity(*item, *itemC);

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

void CDNSModel::statistics(list<DATA_ITEM>& Matrix, vector<string>& redundancy)
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
			float tmp = 0;
			if(null_do_normal)
				tmp = similarity_null_normal(*item, *itemC);
			else
				tmp = similarity(*item, *itemC);

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
			m_center.hostname_redundancy = redundancy;

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

void CDNSModel::clear_redundancy(list<DATA_ITEM>& Matrix, vector<string>& redundancy)
{
	int min=99999;
	list<DATA_ITEM>::iterator pMin;
	list<DATA_ITEM>::iterator pOne = Matrix.begin();
	while(pOne != Matrix.end())
	{
		if(min > pOne->hostname.size() )
		{
			min = pOne->hostname.size();
			pMin = pOne;
		}
		pOne++;
	}

	DATA_ITEM dMin = *pMin;

	vector<string>::iterator pItem = dMin.hostname.begin();
	while(pItem != dMin.hostname.end())
	{
		pOne = Matrix.begin();
		while(pOne != Matrix.end())
		{
			vector<string>::iterator pP = find(pOne->hostname.begin(), pOne->hostname.end(), *pItem);
			if(pP == pOne->hostname.end())
				break;
			pOne++;
		}

		if(pOne == Matrix.end())
		{//delete
				redundancy.push_back(*pItem);
				pOne = Matrix.begin();
				while(pOne != Matrix.end())
				{
					vector<string>::iterator pP = find(pOne->hostname.begin(), pOne->hostname.end(), *pItem);
					if(pP != pOne->hostname.end())
						pOne->hostname.erase(pP);
					pOne++;
				}
		}

		pItem++;
	}

	pOne = Matrix.begin();
	while(pOne != Matrix.end())
	{
		if(pOne->hostname.size() == 0)
		{
			Matrix.erase(pOne);
			pOne = Matrix.begin();
			continue;
		}
		pOne++;
	}
}

bool CDNSModel::generate(string site, string user, list<DATA_ITEM>& Matrix)
{
	m_site = site;
	m_user = user;

	vector<string> redundancy;
	if(null_do_normal)
		clear_redundancy(Matrix, redundancy);
	statistics(Matrix, redundancy);
}

void CDNSModel::clear_same_hostname(const vector<string>& constHostname, vector<string>& changeHostname)
{
	vector<string>::const_iterator pOne = constHostname.begin();
	while(pOne != constHostname.end())
	{
		vector<string>::iterator pP = find(changeHostname.begin(), changeHostname.end(), *pOne);
		if(pP != changeHostname.end())
			changeHostname.erase(pP);

		pOne++;
	}
}

CResult CDNSModel::anomie(DATA_ITEM& item)
{
	CResult Rtn;
	float si = 0;

	DATA_ITEM testItem = item;

	clear_same_hostname(m_center.hostname_redundancy, testItem.hostname);

	if(null_do_normal)
		si = similarity_null_normal(testItem, m_center);
	else
		si = similarity(testItem, m_center);
	
	if(si >= m_min_similarity)
		Rtn.m_anomie = false;
	else
		Rtn.m_anomie = true;

	Rtn.m_similarity = si;
	Rtn.m_user = testItem.user;

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
	cout<<endl;
	cout<<"size:"<<m_size<<endl;
	cout<<"max_similarity:"<<m_max_similarity<<endl;
	cout<<"min_similarity:"<<m_min_similarity<<endl;
	cout<<"center item:"<<endl;
	m_center.info();
	cout<<endl;
}


