#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
using namespace std;
int main()
{
	char tmp[128]={0};
	sprintf(tmp, "./wangXXXXXX");
	int fd = mkstemp(tmp);
	write(fd, "1234567890", 10);
	close(fd);

	string tmpnam="./strXXXXXX";
	fd = mkstemp((char*)tmpnam.c_str());
	write(fd, "1234567890", 10);
	close(fd);
}
