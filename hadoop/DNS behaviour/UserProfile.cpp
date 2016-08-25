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
			rslt = (*f).second.anomie(*item);
			result.push_back(rslt);
		}
		else
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
		CDNSModel model;
		model.generate(site, (*matr).first, (*matr).second);
		model_list.insert(make_pair((*matr).first, model));

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
