#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;
int main()
{
	char tmp[128]={0};
	sprintf(tmp, "./wangXXXXXX");
	char* pname = mktemp(tmp);
	cout<<pname<<endl;
}
