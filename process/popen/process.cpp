#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;

int main()
{
	//FILE* process_file  = popen("/usr/bin/ls /home/sxis", "r");
	//FILE* process_file  = popen("/usr/bin/top -n 1 | wc", "r");
	FILE* process_file  = popen("/usr/bin/ls /home/wzl", "r");
	if(process_file == NULL)
	{
		cout<<"popen error"<<endl;
		return 0;
	}

	char buf[256];
	string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		msg += buf;
	}

	cout<<msg;
	pclose(process_file);
}
