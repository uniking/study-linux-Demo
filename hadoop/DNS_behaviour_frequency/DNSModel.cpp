#include "DNSModel.hpp"

extern bool null_do_normal;

CDNSModel::CDNSModel()
{
	m_site = "";
	m_user = "";
}

void CDNSModel::statistics(list<DATA_ITEM>& Matrix, vector<string>& redundancy)
{
	m_size = Matrix.size();

	list<DATA_ITEM>::iterator item = Matrix.begin();
	while(item != Matrix.end())
	{
		map<string, map<string, long>>::iterator hp = item->hostname_plot.begin();
		while(hp != item->hostname_plot.end())
		{
			long max=0;
			long min;
			long count=0;
			float mean;
			float sd;
			map<string, long>::iterator ho = hp->second.begin();
			max = ho->second;
			min = ho->second;
			while(ho != hp->second.end())
			{
				if(ho->second > max)
					max = ho->second;
				if(ho->second < min)
					min = ho->second;
				count+=ho->second;

				ho++;
			}

			mean = count/hp->second.size();

			CHostnameHav tmp;
			tmp.min = min;
			tmp.max = max;
			tmp.mean = mean;
			tmp.sd = 0;

			m_models.insert(make_pair(hp->first, tmp));
			
			hp++;
		}
		
		item++;
	}
}

void CDNSModel::clear_redundancy(list<DATA_ITEM>& Matrix, vector<string>& redundancy)
{
/*
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
					//	pOne->hostname.erase(pP);
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
*/
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

	map<string, map<string, long>>::iterator hn = item.hostname_plot.begin();
	while(hn != item.hostname_plot.end())
	{
		map<string, CHostnameHav>::iterator mh = m_models.find(hn->first);
		if(mh == m_models.end())
		{//anomie
			map<string, long>::iterator dc = hn->second.begin();
			while(dc != hn->second.end())
			{
				CHostResult tmp;
				tmp.m_inmodel = false;
				tmp.m_anomie = true;				
				Rtn.m_rsult.insert(make_pair(dc->first, tmp));
				dc++;
			}
		}
		else
		{
			map<string, long>::iterator dc = hn->second.begin();
			while(dc != hn->second.end())
			{
				CHostResult tmp;
				tmp.m_inmodel = true;

				if(dc->second > mh->second.max)
					tmp.m_anomie = true;
				else
					tmp.m_anomie = false;

				Rtn.m_rsult.insert(make_pair(dc->first, tmp));
			
				dc++;
			}
		}

		hn++;
	}

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
	cout<<endl;
}


