#include <stdio.h>

#include <map>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
	int a = 0;
	double b = 1.23;
	char* pstr="hello world";

	string info;
	info += a;
	info += " ";
	info += b;
	info += " ";
	info += pstr;
	cout<<info<<endl;

	ostringstream oss;
	oss<<a<<" "<<b<<" "<<pstr;
	info=oss.str();
	cout<<info<<endl;

    stringstream test("abc<aaa@qqq.com>kkk");
    string sub;
    getline(test, sub, '<');
    cout<<sub<<endl;

    getline(test, sub, '>');
    cout<<sub<<endl;



    string utf_1="中国";
    string utf_2="中国";
    if(utf_1 == utf_2)
        cout<<utf_1<<"=="<<utf_2<<endl;

    string find_s="/root/abc/ef.txt";
    int fn = find_s.find_last_of('/');
    string sub_n = find_s.substr(fn+1);
    cout<<fn<<" "<<sub_n<<endl;

}
