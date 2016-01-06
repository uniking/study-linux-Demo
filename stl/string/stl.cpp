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
}
