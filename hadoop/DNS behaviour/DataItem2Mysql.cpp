#include "DataItem2Mysql.hpp"
#include "DataItem.hpp"

#define ITEM_TABLE_NAME "dnsItem"

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
	string hostname;
	int ret=0;

	vector<string>::iterator pHostName = data.hostname.begin();
	while(pHostName != data.hostname.end())
	{
		hostname+=*pHostName;
		hostname+=" ";
		pHostName++;
	}

	int bLength = hostname.size() + 128;
	char* pQuery = (char*)malloc(bLength);
	if(pQuery)
	{
		snprintf(pQuery, bLength, "insert into %s(time, sip, hostname) value(\'%s\',  \'%s\', \'%s\')", ITEM_TABLE_NAME, data.strTime.c_str(), data.sip.c_str(), hostname.c_str());
		ret = mysql_real_query(mysql, pQuery, (unsigned int)strlen(pQuery));
		if(ret)
		{
			printf("mysql_real_query error\n");
		}

		free(pQuery);
	}
	else
	{
		printf("malloc error\n");
		ret = -1;
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

		if(itemList.size() > 0)
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

int get_item_list_by_sip(MYSQL* mysql, const string& sip, list<string>& ignoreDay, list<DATA_ITEM>& itemList)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select time, hostname from %s where sip=\'%s\'", ITEM_TABLE_NAME, sip.c_str());

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{
		DATA_ITEM di;
		di.user = sip;
		di.sip = sip;

		for(t=0; t<mysql_num_fields(res); t++)
		{
			if(t == 0)
			{
				di.strTime = row[t];
				char buf[11];
				memcpy(buf, row[t], 10);
				buf[10]=0;

				if(ignoreDay.size() == 0  || find(ignoreDay.begin(), ignoreDay.end(), buf) == ignoreDay.end())
					;
				else
					break;
			}
			else if(t == 1)
				Tokenize(string(row[t]), di.hostname, string(" "));
			else
				printf("query error\n");
		}

		if(di.hostname.size() > 0)
			itemList.push_back(di);
	}

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

int get_all_item_by_one_day(MYSQL* mysql, const string& day, list<DATA_ITEM>& itemList)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select time, sip, hostname from %s where DATE(time) = DATE(\"%s\")", ITEM_TABLE_NAME, day.c_str());

	t = mysql_real_query(mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(mysql);
	while(row = mysql_fetch_row(res))
	{
		DATA_ITEM di;

		for(t=0; t<mysql_num_fields(res); t++)
		{
			if(t == 0)
				di.strTime = row[t];
			else if(t == 1)
			{
				di.sip = row[t];
				di.user = di.sip;
			}
			else if(t == 2)
				Tokenize(string(row[t]), di.hostname, string(" "));
			else
				printf("query error\n");
		}

		if(di.hostname.size() > 0)
			itemList.push_back(di);
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
