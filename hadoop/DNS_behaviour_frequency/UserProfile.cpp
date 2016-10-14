#include "UserProfile.hpp"

void CUserProfile::anomie(list<DATA_ITEM>& data, list<CResult>& result)
{
	list<DATA_ITEM>::iterator item = data.begin();
	while(item != data.end())
	{
		map<string, CDNSModel>::iterator f = model_list.find((*item).user);
		CResult rslt;
		if(f != model_list.end())
		{
			if(item->hostname_plot.size() == 0)
			{
				rslt.m_user=item->user;
				rslt.m_inmodel = true;
				rslt.m_anomie = false;
			}
			else
			{
				rslt = (*f).second.anomie(*item);
			}
		}
		else
		{
			rslt.m_user=item->user;
			rslt.m_inmodel=false;
		}
		result.push_back(rslt);

		item++;
	}
}

bool CUserProfile::generate(string site, list<DATA_ITEM>& Matrix)
{
	map<string, list<DATA_ITEM> > dispatch;

	list<DATA_ITEM>::iterator item = Matrix.begin();
	while(item != Matrix.end())
	{
		map<string, list<DATA_ITEM> >::iterator one = dispatch.find( (*item).user );
		if(one != dispatch.end())
		{
			(*one).second.push_back(*item);
		}
		else
		{
			list<DATA_ITEM> tmp;
			tmp.push_back(*item);
			dispatch.insert(make_pair((*item).user,tmp));
		}

		item++;
	}

	map<string, list<DATA_ITEM> >::iterator matr = dispatch.begin();
	while(matr != dispatch.end())
	{
		if((*matr).second.size() > 3)
		{
			CDNSModel model;
			model.generate(site, (*matr).first, (*matr).second);
			model_list.insert(make_pair((*matr).first, model));
		}

		matr++;
	}

	return model_list.size() == 0?false:true;
}

bool CUserProfile::generate(string site, map<string, list<DATA_ITEM> >& Matrix)
{
	map<string, list<DATA_ITEM> >::iterator matr = Matrix.begin();
	while(matr != Matrix.end())
	{
		CDNSModel model;
		model.generate(site, (*matr).first, (*matr).second);
		model_list.insert(make_pair((*matr).first, model));

		matr++;
	}

	return model_list.size() == 0?false:true;
}

void CUserProfile::info()
{
	map<string, CDNSModel>::iterator one =  model_list.begin();
	while(one != model_list.end())
	{
		(*one).second.info();
		one++;
	}
}

bool CUserProfile::sort(string site, map<string, list<DATA_ITEM> >& Matrix, map<string, multimap<float, DATA_ITEM> >& outMatrix)
{
	if(model_list.size() == 0)
	{
		printf("first generate model\n");
		return false;
	}

	map<string, list<DATA_ITEM> >::iterator matr = Matrix.begin();
	while(matr != Matrix.end())
	{
		multimap<float, DATA_ITEM> sortItem;
		list<CResult> result;
		anomie(matr->second, result);
		list<CResult>::iterator pR = result.begin();
		list<DATA_ITEM>::iterator pDI = matr->second.begin();
		while(pR != result.end() && pDI != matr->second.end())
		{
			sortItem.insert(make_pair(pR->m_similarity, *pDI));

			pR++;
			pDI++;
		}

		outMatrix.insert(make_pair(matr->first, sortItem));
		

		matr++;
	}

	//calc mean variance
	map<string, multimap<float, DATA_ITEM> >::iterator pUser=outMatrix.begin();
	while(pUser != outMatrix.end())
	{
		float sum=0;
		multimap<float, DATA_ITEM>::iterator pItem = pUser->second.begin();
		while(pItem != pUser->second.end())
		{
			sum += pItem->first;
			pItem++;
		}
		float mean = (sum-1)/(pUser->second.size()-1);

		pItem = pUser->second.begin();
		while(pItem != pUser->second.end())
		{
			float sd = pow(pItem->first - mean, 2);
			pItem->second.mean = mean;
			pItem->second.variance = sd;
			if(mean == 0)
				pItem->second.CV = 0;
			else
				pItem->second.CV = sd/mean;

			pItem++;
		}

		pUser++;
	}
}
