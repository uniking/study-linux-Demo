#include <unistd.h>
#include <time.h>

#include <re2/re2.h>
#include <re2/stringpiece.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

timespec diff(timespec start, timespec end);
void printlog(bool bT)
{
	if(bT)
	{
		cout<<"match ok"<<endl;
	}
	else
	{
		cout<<"match error"<<endl;
	}
}


int main()
{
	ifstream inFile("./phone.txt");
	string sContent;
	ostringstream os;
	os << inFile.rdbuf();
	sContent = os.str();
	bool bT;
   // cout<<sContent<<endl;

	//RE2 sear("");
	//int s = clock () ;
	//bT = RE2::FullMatch("hello1", "hello"); //完全匹配，差一个字母也不行
	//bT = RE2::FullMatch("hello", "hello");
	//int e = clock () ;
	//printlog(bT);
	//cout<<"tiem="<<e-s<<endl;

	timespec time1, time2;
	//int s = clock () ;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	//bT = RE2::PartialMatch((char*)(sContent.c_str()), "123456789123456789");
	//sleep(10);
	//int e = clock () ;

    string strTemp="";
    int i=0;
   re2::StringPiece input((char*)sContent.c_str());
   //re2::StringPiece input("11111111111111111111111111111");
	//while(RE2::FindAndConsume(&input, "(\\d{11})", &strTemp))
	//while(RE2::FindAndConsume(&input, "(\\d{3}\\-\\d{7})", &strTemp))
   // while(RE2::FindAndConsume(&input, "(函数|执行|)", &strTemp))

    // ([1-9]\d{5}(?!\d))

    cout<<"count:"<<sContent<<endl;
    //while(RE2::FindAndConsume(&input, "(<\\w+>)", &strTemp))
    while(RE2::FindAndConsume(&input, "([1-9]\d{5}(?!\d))", &strTemp))
	{
		i++;
		cout<< endl<<strTemp.c_str()<<endl;
	}


/*
	int i=0;
	char temp[32] = {0};
	char c1=0;
	string strTemp="";
	long di=0;
	re2::StringPiece input("xxxxx132874525652236578xxxxxxx77777777778xxxx010-5784125xxxxx020-1234567");
	//while(RE2::FindAndConsume(&input, "(\\d{18})", &di))
	while(RE2::FindAndConsume(&input, "(\\d{3}\\-\\d{7})", &strTemp))
	{
		i++;
		cout<<i<<" "<<temp <<" "<<di<<" "<< strTemp.c_str()<<endl;
		temp[0]=0;
	}
*/


/*
	re2::StringPiece input2("xxxxx132874525652236578xxxxxxx77777777778xxxx010-5784125xxxxx020-1234567");
	while(RE2::FindAndConsume(&input2, "(\\d{18})", &strTemp))
	{
		i++;
		cout<<i<<" "<<temp <<" "<<di<<" "<< strTemp.c_str()<<endl;
		temp[0]=0;
	}

	if(bT =RE2::PartialMatch("xxxxx132874525652236578xxxxxxx77777777778xxxx010-5784125xxx","(\\d{3}\\-\\d{7})"))
	{
		printlog(bT);
	}
*/
	
	//while(RE2::Consume(&input, "123456", temp))
	//{
	//	i++;
	//	cout<<i<<" ";
	//}

/*
	cout<<endl;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	printlog(bT);
	//cout<<"tiem="<<e-s<<"e="<<e<<" s="<<s<<endl;
	//cout<<diff(time1, time2).tv_sec<<":"<<diff(time1, time2).tv_nsec<<endl;
	cout<<"time="<<time2.tv_nsec-time1.tv_nsec<<endl;
*/

	//int num=0;
	//string ph;
	//RE2::FullMatch("sdkljfu39khfjdfksdjf", "(\\d+)", &num);
	//cout<<ph.c_str()<<" "<<num<<" ";
	//printlog(bT);

	//RE2::FullMatch("hello", "h.*o");

	//匹配多个关键字

/*
	if(bT =RE2::PartialMatch("中华人们共和国，北京市，海淀区","北京|海淀"))
	{
		printf("多关键字:");
		printlog(bT);
	}
*/

/*
	//多次抽取，多个关键字
	re2::StringPiece input3("中华人们共和国，北京市，海淀区");
	while(RE2::FindAndConsume(&input3, "(北京|海淀)", &strTemp))
	{
		i++;
		cout<<i<<" "<<temp <<" "<<di<<" "<< strTemp.c_str()<<endl;
		temp[0]=0;
	}
*/


	//返回匹配的偏移量
	std::string test="<wangzhl@suninfo.com>";
	re2::StringPiece result;
	if( RE2::PartialMatch(test, "(<\\w+>)", &result) )
		std::cout<<"first at "<< result.data()- test.data() << endl;
	

	return 0;
}
