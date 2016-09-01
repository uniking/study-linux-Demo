#include "QueryByTime.hpp"

MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;

#define TABLE_NAME "dns"

int init_mysql()
{
	mysql_init(&mysql);
	
	if(!mysql_real_connect(&mysql, "localhost", "root", "abcd", "DNSdata", 0, NULL, 0) )
	{
		printf("mysql_real_connect error\n");
		return 1;
	}

	return 0;
}

int get_day_by_database(set<string>& daySet, list<string>& ignoreDay)
{
	int t,r;
	char query[128];
	snprintf(query, 128, "select distinct time from %s", TABLE_NAME);
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			char buf[11];
			memcpy(buf, row[t], 10);
			buf[10]=0;

			if(ignoreDay.size() == 0)
				daySet.insert(string(buf));
			else
			{
				if( find(ignoreDay.begin(), ignoreDay.end(), buf) == ignoreDay.end())
					daySet.insert(string(buf));
			}
		}
	}

	return 0;
}

int get_sip_by_day(set<string>& sipSet, const char* day)
{// select sip from dns where DATE(time) = DATE(\"2016-08-11\")
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select distinct sip from %s where DATE(time) = DATE(\"%s\")", TABLE_NAME, day);

	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			sipSet.insert(string(row[t]));
		}
	}

	return 0;
}

int get_hostname_by_sip_day(set<string>& hostnameSet, const char* sip, const char* day)
{//select hostname from dns where DATE(time) = DATE("2016-08-11") and sip="192.168.220.10";
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select distinct hostname from %s where DATE(time) = DATE(\"%s\") and sip=\"%s\"", TABLE_NAME, day, sip);

	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			hostnameSet.insert(string(row[t]));
		}
	}

	return 0;
}

bool hostname_filtrate(string hostname)
{
	if(hostname.find("www.") != string::npos ||
		hostname.find("mail.") != string::npos ||
		hostname.find("mailbox.") != string::npos ||
		hostname.find("bbs.") != string::npos ||
		hostname.find("baike.") != string::npos ||
		hostname.find("dict.") != string::npos ||
		hostname.find("tieba.") != string::npos ||
		hostname.find("news.") != string::npos ||
		hostname.find("cn.") != string::npos)
		return false;

	return true;
}

/*
Matrix <user, items>
*/
int database_to_item(string site, int e, map<string, list<DATA_ITEM> >& Matrix, list<string>& ignoreDay,bool filtrate)
{
	if(init_mysql())
		return 1;

	set<string> daySet;
	list<DATA_ITEM> itemList;
	get_day_by_database(daySet, ignoreDay);
	set<string>::iterator DayIter = daySet.begin();
	while(DayIter != daySet.end())
	{		
		set<string> sipSet;
		get_sip_by_day(sipSet, (*DayIter).c_str());
		set<string>::iterator SipIter = sipSet.begin();
		while(SipIter != sipSet.end())
		{
			DATA_ITEM dIt;
			dIt.e = e;
			dIt.sip = *SipIter;
			dIt.user = *SipIter;
			dIt.strTime = *DayIter;
			set<string> hostnameSet;
			get_hostname_by_sip_day(hostnameSet, (*SipIter).c_str(), (*DayIter).c_str());
			set<string>::iterator HostnameIter = hostnameSet.begin();
			while(HostnameIter != hostnameSet.end())
			{
				if(filtrate)
				{
					if(!hostname_filtrate(*HostnameIter))
						dIt.hostname.push_back(*HostnameIter);
				}
				else
					dIt.hostname.push_back(*HostnameIter);
				HostnameIter++;
			}

			itemList.push_back(dIt);
			SipIter++;
		}
	
		
		DayIter++;
	}

	//separate by user
	set<string> name_list;
	list<DATA_ITEM>::iterator oneItem =  itemList.begin();
	while(oneItem != itemList.end())
	{
		name_list.insert((*oneItem).user);
		oneItem++;
	}

	set<string>::iterator name = name_list.begin();
	while(name != name_list.end())
	{
		if((*name).size() == 0)
		{
			name++;
			continue;
		}

		list<DATA_ITEM> tmp_list;
		
		oneItem =  itemList.begin();
		while(oneItem != itemList.end())
		{
			if((*oneItem).user == *name && (*oneItem).hostname.size() != 0)
				tmp_list.push_back(*oneItem);
			oneItem++;
		}

		if(tmp_list.size() != 0)
			Matrix.insert(make_pair(*name, tmp_list));
		name++;
	}
	

	

	mysql_free_result(res);
	mysql_close(&mysql);
	return 0;
}

/*
Matrix <user, items>
*/
int get_item_by_one_day(string site, char* day, int e, list<DATA_ITEM>& itemList, bool filtrate)
{
	if(init_mysql())
		return 1;
	
	set<string> sipSet;
	get_sip_by_day(sipSet, day);
	set<string>::iterator SipIter = sipSet.begin();
	while(SipIter != sipSet.end())
	{
		DATA_ITEM dIt;
		dIt.e = e;
		dIt.sip = *SipIter;
		dIt.user = *SipIter;
		set<string> hostnameSet;
		get_hostname_by_sip_day(hostnameSet, (*SipIter).c_str(), day);
		set<string>::iterator HostnameIter = hostnameSet.begin();
		while(HostnameIter != hostnameSet.end())
		{
			if(filtrate)
			{
				if(!hostname_filtrate(*HostnameIter))
					dIt.hostname.push_back(*HostnameIter);
			}
			else
				dIt.hostname.push_back(*HostnameIter);
			HostnameIter++;
		}

		if(dIt.hostname.size() == 0 ||
			dIt.sip.size() == 0)
			;
		else
			itemList.push_back(dIt);
		SipIter++;
	}

	mysql_free_result(res);
	mysql_close(&mysql);
	return 0;
}

int delete_data_by_day(list<string>& ignoreDay)
{
	int t,r;

	if(init_mysql())
		return 1;

	list<string>::iterator pOneDay = ignoreDay.begin();
	while(pOneDay != ignoreDay.end())
	{
		char query[128];
		snprintf(query, 128, "delete from %s where DATE(time) = DATE(\"%s\")", TABLE_NAME, pOneDay->c_str());
		t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
		if(t)
		{
			printf("mysql_real_query error\n");
		}

		pOneDay++;
	}

	mysql_close(&mysql);
	return 0;
}
