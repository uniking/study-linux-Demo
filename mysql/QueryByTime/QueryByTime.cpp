#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <set>
#include <iostream>
using namespace std;

	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* query;
	

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

int get_data_by_day(set<string>& daySet)
{
	int t,r;
	char query[128];
	snprintf(query, 128, "select time from dns");
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

			daySet.insert(string(buf));
		}
	}

	return 0;
}

int get_sip_by_day(set<string>& sipSet, const char* day)
{// select sip from dns where DATE(time) = DATE(\"2016-08-11\")
	int t,r;
	char query[1024];
	snprintf(query, 1024, "select sip from dns where DATE(time) = DATE(\"%s\")", day);

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
	snprintf(query, 1024, "select hostname from dns where DATE(time) = DATE(\"%s\") and sip=\"%s\"", day, sip);

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


/*
127.0.0.1 root abcd
DNSdata dns (id name)
*/
int main()
{
	if(init_mysql())
		return 1;

	set<string> daySet;
	get_data_by_day(daySet);
	set<string>::iterator DayIter = daySet.begin();
	while(DayIter != daySet.end())
	{
		cout<<"--------------"<<*DayIter<<"----------------"<<endl;
		set<string> sipSet;
		get_sip_by_day(sipSet, (*DayIter).c_str());
		set<string>::iterator SipIter = sipSet.begin();
		while(SipIter != sipSet.end())
		{
			cout<<*SipIter<<endl;
			set<string> hostnameSet;
			get_hostname_by_sip_day(hostnameSet, (*SipIter).c_str(), (*DayIter).c_str());
			set<string>::iterator HostnameIter = hostnameSet.begin();
			cout<<"-------hostname size = "<<hostnameSet.size()<<endl;
			while(HostnameIter != hostnameSet.end())
			{
				cout<<*HostnameIter<<"  ";
				HostnameIter++;
			}
			cout<<endl;

			SipIter++;
		}
		DayIter++;
	}

	mysql_free_result(res);
	mysql_close(&mysql);
	return 0;
}
