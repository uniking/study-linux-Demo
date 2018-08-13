#include <string>
#include <stdio.h>
using namespace std;

void exec_cmd(const char* cmd)
{
	printf("exec_cmd:%s\n", cmd);
	FILE* process_file  = popen(cmd, "r");
	if(process_file == NULL)
	{
		return;
	}

	char buf[256];
	string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		//printf("wget info: %s\n", buf);
		msg += buf;
	}

	printf(msg.c_str());
	pclose(process_file);

}

void printQrCode(const char* str)
{
	char cmd[256];
	snprintf(cmd, 255, "qrencode -t UTF8 %s", str);
	exec_cmd(cmd);
}
