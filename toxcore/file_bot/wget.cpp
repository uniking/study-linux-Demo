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

pthread_t ntid;

typedef struct tox_wget {
    Tox *tox;
    uint32_t friend_number;
    char* url;
} TOX_WGET;

void * thr_fn(void* arg)
{
	TOX_WGET* tw = (TOX_WGET*)arg;

	printf("wget %s\n", tw->url);
	char cmd[1024];
	snprintf(cmd, 1024, "wget %s", tw->url);
	FILE* process_file  = popen(cmd, "r");
	if(process_file == NULL)
	{
		return NULL;
	}

	char buf[256];
	string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		printf("wget info: %s\n", buf);
		msg += buf;
	}

	pclose(process_file);

	tox_friend_send_message(tw->tox, tw->friend_number, TOX_MESSAGE_TYPE_NORMAL, (const uint8_t*)"wget finish!\n", strlen("wget finish!\n"), NULL);

	printf("wget thread exit\n");
	free((void*)(tw->url));
	free(arg);

	return NULL;
}

void wget(Tox *tox, uint32_t friend_number, char* url)
{
	TOX_WGET* tw = (TOX_WGET*)malloc(sizeof(TOX_WGET));
	tw->tox = tox;
	tw->friend_number = friend_number;
	int url_len = strlen(url);
	char* purl = (char*)malloc(url_len+1);
	tw->url = purl;
	strcpy(tw->url, url);
	tw->url[url_len+1] = 0;

	pthread_t ntid = 0;
	pthread_create(&ntid, NULL, thr_fn, tw);
}
