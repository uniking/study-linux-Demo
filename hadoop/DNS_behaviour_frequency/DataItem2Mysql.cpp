#include "DataItem2Mysql.hpp"
#include "DataItem.hpp"

#define ITEM_TABLE_NAME "dfItem"

extern int min_count;

// time sip dip count

extern int model_shorttest_cycle;

int init_mysql(MYSQL* mysql)
{
	mysql_init(mysql);
	
	if(!mysql_real_connect(mysql, "localhost", "root", "abcd", "DNSdata", 0, NULL, 0) )
	{
		printf("mysql_real_connect error\n");
		return 1;
	}

	return 0;
}

int insert_item(MYSQL* mysql, DATA_ITEM& data)
{
	int ret=0;


	char headBuffer[512];
	snprintf(headBuffer,512, "insert into %s(time, sip, hostname, count) value", ITEM_TABLE_NAME);
	string Query = headBuffer;

	map<string, long>::iterator pHostName = data.hostname.begin();
	while(pHostName != data.hostname.end())
	{//first value
		char buf[1024];
		if(1024 <= snprintf(buf, 1024, "(\'%s\',  \'%s\', \'%s\', \'%d\')", \
						data.strTime.c_str(), data.sip.c_str(), pHostName->first.c_str(), pHostName->second) )
		{
			pHostName++;
			continue;
		}
		else
		{
			Query += buf;
			pHostName++;
			break;
		}
	}

	while(pHostName != data.hostname.end())
	{
		char buf[1024];
		if(1024 <= snprintf(buf, 1024, ",(\'%s\',  \'%s\', \'%s\', \'%d\')", \
						data.strTime.c_str(), data.sip.c_str(), pHostName->first.c_str(), pHostName->second) )
		{
			pHostName++;
			continue;
		}
		else
			Query += buf;
		
		pHostName++;
	}


	ret = mysql_real_query(mysql, Query.c_str(), Query.size());
	if(ret)
	{
		printf("mysql_real_query error\n");
	}

	return ret;
}

int query_item(MYSQL* mysql, list<string>& ignoreDay, map<string, list<DATA_ITEM> >& Matrix)
{
	set<string> sipSet;
	get_sip(mysql, sipSet);

	set<string>::iterator pSip = sipSet.begin();
	while(pSip != sipSet.end())
	{
		list<DATA_ITEM> itemList;
		get_item_list_by_sip(mysql, *pSip, ignoreDay, itemList);
		Matrix.insert(make_pair(*pSip, itemList));
		pSip++;
	}

	return 0;
}



int get_sip(MYSQL* mysql, set<string>& sipSet)
{// select sip from dns where DATE(time) = DATE(\"2016-08-11\")
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select distinct sip from %s", ITEM_TABLE_NAME);

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			sipSet.insert(string(row[t]));
		}
	}

	return 0;
}

int get_sip_by_date(MYSQL* mysql, const string& day, set<string>& sipSet)
{// select sip from dns where DATE(time) = DATE(\"2016-08-11\")
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select distinct sip from %s where DATE(time) = DATE(\"%s\")", ITEM_TABLE_NAME, day.c_str());

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			sipSet.insert(string(row[t]));
		}
	}

	return 0;
}

int get_item_list_by_sip(MYSQL* mysql, const string& sip, list<string>& ignoreDay, list<DATA_ITEM>& itemList)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select time, hostname, count from %s where sip=\'%s\' and count>%d", ITEM_TABLE_NAME, sip.c_str(), min_count);

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	DATA_ITEM di;
	di.user = sip;
	di.sip = sip;
	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{

		list<string>::iterator pI = find(ignoreDay.begin(), ignoreDay.end(), row[0]);
		if(pI != ignoreDay.end())
			continue;

/*
		bool bIg = false;
		list<string>::iterator pI = ignoreDay.begin();
		while(pI != ignoreDay.end())
		{
			if(*pI == row[0])
			{
				bIg = true;
				break;
			}
			pI++;
		}
		if(bIg == true)
			continue;
*/

		map<string, map<string, long>>::iterator pF = di.hostname_plot.find(row[1]);
		if(pF == di.hostname_plot.end())
		{
			map<string, long> tmp;
			tmp.insert(make_pair(row[0], atoi(row[2])));
			di.hostname_plot.insert(make_pair(row[1], tmp));
		}
		else
			pF->second.insert(make_pair(row[0], atoi(row[2])));
	}

	itemList.push_back(di);

	return 0;
}

int get_item_list_by_sip_and_day(MYSQL* mysql, const string& sip, const string& day, list<DATA_ITEM>& itemList)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select time, hostname, count from %s where sip=\'%s\' and DATE(time) = DATE(\"%s\") and count > %d", ITEM_TABLE_NAME, sip.c_str(), day.c_str(), min_count);

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	DATA_ITEM di;
	di.user = sip;
	di.sip = sip;
	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{		
		map<string, map<string, long>>::iterator pF = di.hostname_plot.find(row[1]);
		if(pF == di.hostname_plot.end())
		{
			map<string, long> tmp;
			tmp.insert(make_pair(row[0], atoi(row[2])));
			di.hostname_plot.insert(make_pair(row[1], tmp));
		}
		else
			pF->second.insert(make_pair(row[0], atoi(row[2])));
	}

	itemList.push_back(di);

	return 0;
}


void Tokenize(const string& str,
        vector<string>& tokens,
        const string& delimiters)
{  
    // Skip delimiters at beginning.  
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);  
    // Find first "non-delimiter".  
    string::size_type pos     = str.find_first_of(delimiters, lastPos);  
    while (string::npos != pos || string::npos != lastPos)  
    {  
        // Found a token, add it to the vector.  
        tokens.push_back(str.substr(lastPos, pos - lastPos));  
    //     Skip delimiters.  Note the "not_of"  
        lastPos = str.find_first_not_of(delimiters, pos);  
        // Find next "non-delimiter"  
        pos = str.find_first_of(delimiters, lastPos);  
    }  
}

int get_all_item_by_one_day(MYSQL* mysql, const string& day, map<string, list<DATA_ITEM> >& Matrix)
{
	list<string> ignoreDay;
	set<string> sipSet;
	get_sip_by_date(mysql, day, sipSet);

	set<string>::iterator pSip = sipSet.begin();
	while(pSip != sipSet.end())
	{
		list<DATA_ITEM> itemList;
		get_item_list_by_sip_and_day(mysql, *pSip, day, itemList);
		Matrix.insert(make_pair(*pSip, itemList));
		pSip++;
	}

	return 0;
}

int get_day_set_from_item_database(MYSQL* mysql, list<string>& daySet)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[128];
	snprintf(query, 128, "select distinct time from %s", ITEM_TABLE_NAME);
	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			char buf[11];
			memcpy(buf, row[t], 10);
			buf[10]=0;

			daySet.push_back(string(buf));
		}
	}

	return 0;
}
