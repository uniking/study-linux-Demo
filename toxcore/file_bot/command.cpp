#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>

#include <tox/tox.h>

#include <string>
using namespace std;

/*
linux 内核中没有实现pthread更能，所以内核中也没有pthead.h文件，
应用层库实现，链接时需要使用 -lpthread  (pthread posix thread)
pthread_create调用内核函数clone
*/

typedef struct tox_wget {
    Tox *tox;
    uint32_t friend_number;
    char* cmd;
} TOX_WGET;

void * thr_cmd(void* arg)
{
	TOX_WGET* tw = (TOX_WGET*)arg;

	//printf("thr_cmd %s\n", tw->cmd);
	FILE* process_file  = popen(tw->cmd, "r");
	if(process_file == NULL)
	{
		return NULL;
	}

	char buf[256];
	//string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		//printf("wget info: %s\n", buf);
		tox_friend_send_message(tw->tox, tw->friend_number, TOX_MESSAGE_TYPE_NORMAL, (const uint8_t*)buf, strlen(buf), NULL);
		//msg += buf;
	}

	pclose(process_file);

	//不能一次发送太多的message
	//tox_friend_send_message(tw->tox, tw->friend_number, TOX_MESSAGE_TYPE_NORMAL, (const uint8_t*)msg.c_str(), msg.length(), NULL);

	//printf("wget thread exit\n");
	free((void*)(tw->cmd));
	free(arg);

	return NULL;
}

void cmd(Tox *tox, uint32_t friend_number, const char* cmd)
{
	//printf("cmd %s\n", cmd);
	TOX_WGET* tw = (TOX_WGET*)malloc(sizeof(TOX_WGET));
	tw->tox = tox;
	tw->friend_number = friend_number;
	int cmd_len = strlen(cmd);
	char* pcmd = (char*)malloc(cmd_len+1);
	tw->cmd = pcmd;
	strcpy(tw->cmd, cmd);
	tw->cmd[cmd_len+1] = 0;

	pthread_t ntid_cmd = 0;
	pthread_create(&ntid_cmd, NULL, thr_cmd, tw);
}
