#include "QueryByTime.hpp"
#include "UserProfile.hpp"
#include "Result.hpp"

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
	//snprintf(day, 32, "2016-08-12");
	get_yesterday(day, 32);
	get_item_by_one_day("sxis", day, 24, data, false);
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

int main()
{
	map<string, list<DATA_ITEM> > Matrix;
	list<string> ignoreDay;
	char day[32];
	get_yesterday(day, 32);
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
