#include "QueryByTime.hpp"
#include "UserProfile.hpp"
#include "Result.hpp"
#include "DataItem2Mysql.hpp"

void get_yesterday(char* day, int length)
{
	tzset();
	time_t timer1;
	struct tm* t;

	timer1 = time(NULL);
	timer1 -= (24*60*60);
	t = localtime(&timer1);
	snprintf(day, length, "%04d-%02d-%02d",t->tm_year+1900, t->tm_mon+1, t->tm_mday);
}


void test(CUserProfile& up)
{
	list<DATA_ITEM> data;
	list<CResult> result;

	DATA_ITEM item;
	item.user="192.168.220.207";
	item.sip="192.168.220.207";
	item.hostname.push_back("d.x.baidu.com");
	item.hostname.push_back("rch.discuz.qq.com");
	item.hostname.push_back("rl.mail.qq.com");
	item.hostname.push_back("uqian.qq.com");
	item.hostname.push_back("v.qq.com");
	item.hostname.push_back("wpa.qq.com");

	data.push_back(item);

	up.anomie(data, result);
	list<CResult>::iterator r = result.begin();
	while(r != result.end())
	{
		(*r).info();
		r++;
	}
	cout<<endl;
}

void test2(CUserProfile& up)
{
	list<DATA_ITEM> data;
	list<CResult> result;
	char day[32];
	snprintf(day, 32, "2016-08-29");
	//get_yesterday(day, 32);
	get_item_by_one_day("sxis", day, 24, data, false);

	cout<<"test item inf:"<<endl;
	list<DATA_ITEM>::iterator pI = data.begin();
	while(pI != data.end())
	{
		(*pI).info();
		pI++;
	}

	up.anomie(data, result);
	cout<<"result:"<<endl;
	list<CResult>::iterator r = result.begin();
	while(r != result.end())
	{
		(*r).info();
		r++;
	}
	cout<<endl;
}

void test3(MYSQL* mysql, CUserProfile& up)
{
	list<DATA_ITEM> data;
	list<CResult> result;
	char day[32];
	snprintf(day, 32, "2016-08-29");
	//get_yesterday(day, 32);
	get_all_item_by_one_day(mysql, string(day), data);

	cout<<"test item inf:"<<endl;
	list<DATA_ITEM>::iterator pI = data.begin();
	while(pI != data.end())
	{
		(*pI).info();
		pI++;
	}

	up.anomie(data, result);
	cout<<"result:"<<endl;
	list<CResult>::iterator r = result.begin();
	while(r != result.end())
	{
		(*r).info();
		r++;
	}
	cout<<endl;
}

int item2database()
{
	map<string, list<DATA_ITEM> > Matrix;
	list<string> ignoreDay;

	MYSQL mysql;
	init_mysql(&mysql);
	
	database_to_item("sxis", 24, Matrix, ignoreDay, false);

	map<string, list<DATA_ITEM> >::iterator pUserAct = Matrix.begin();
	while(pUserAct != Matrix.end())
	{
		list<DATA_ITEM>::iterator pOneAct = pUserAct->second.begin();
		while(pOneAct != pUserAct->second.end())
		{
			insert_item(&mysql, *pOneAct);
			pOneAct++;
		}

		pUserAct++;
	}

	return 0;
}

int mainTest()
{
	map<string, list<DATA_ITEM> > Matrix;
	list<string> ignoreDay;
	char day[32];
	snprintf(day, 32, "2016-08-29");
	//get_yesterday(day, 32);
	ignoreDay.push_back(day);
	cout<<"database to item"<<endl;
	database_to_item("sxis", 24, Matrix, ignoreDay, false);

	cout<<"generate model"<<endl;
	CUserProfile up;
	up.generate("sxis", Matrix);

	cout<<"test"<<endl;
	up.info();

	//test(up);
	test2(up);

	return 0;
}

int mainTest2()
{
	MYSQL mysql;
	map<string, list<DATA_ITEM> > Matrix;

	list<string> ignoreDay;
	char day[32];
	snprintf(day, 32, "2016-08-29");
	//get_yesterday(day, 32);
	ignoreDay.push_back(day);

	init_mysql(&mysql);
	query_item(&mysql, ignoreDay, Matrix);

	cout<<"generate model"<<endl;
	CUserProfile up;
	up.generate("sxis", Matrix);

	cout<<"test"<<endl;
	up.info();

	test3(&mysql, up);

	return 0;
}

int updateItemData()
{
	map<string, list<DATA_ITEM> > Matrix;
	MYSQL mysql;
	init_mysql(&mysql);
	list<string> ignoreDay;

	get_day_set_from_item_database(&mysql, ignoreDay);

	char day[32];
	//snprintf(day, 32, "2016-08-29");
	get_yesterday(day, 32);
	ignoreDay.push_back(string(day));

	database_to_item("sxis", 24, Matrix, ignoreDay, false);

	map<string, list<DATA_ITEM> >::iterator pUserAct = Matrix.begin();
	while(pUserAct != Matrix.end())
	{
		list<DATA_ITEM>::iterator pOneAct = pUserAct->second.begin();
		while(pOneAct != pUserAct->second.end())
		{
			insert_item(&mysql, *pOneAct);
			pOneAct++;
		}

		pUserAct++;
	}
}

int cleanDataWarehouse()
{
	MYSQL mysql;
	init_mysql(&mysql);
	list<string> ignoreDay;

	get_day_set_from_item_database(&mysql, ignoreDay);
	delete_data_by_day(ignoreDay);

	return 0;
}

void info(char* pro)
{
cout<<pro<<endl<<" -u update item data"<<endl<<" -d discover anomie"<<endl
			<<" -c clean data warehouse"<<endl;
}

int main(int argn, char* argv[])
{
	int ret = 0;
	if(argn == 1)
	{
		info(argv[0]);
		return -1;
	}
	
	string para=argv[1];
	string update="-u";
	string clean="-c";
	string discover="-d";

	if(para == update)
	{
		ret = updateItemData();
	}
	else if(para == clean)
	{
		//ret = cleanDataWarehouse();
	}
	else if(para == discover)
	{
		ret = mainTest2();
	}
	else
	{
		info(argv[0]);
		ret = -1;
	}

	//item2database();
	//mainTest2();

	return ret;
}
