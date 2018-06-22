#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>

int g_inotify = 0;
int g_run = 1;
#define MAX_BUF_SIZE 1024

void inotify(const char* path, int mask)
{
	printf("start watch:%s mask=%d\n", path, mask);
	struct inotify_event* pEvent;
	char buf[MAX_BUF_SIZE];
	g_inotify = inotify_init();
	inotify_add_watch(g_inotify, path, mask);
	while(g_run == 1)
	{
		size_t len = read(g_inotify, buf , MAX_BUF_SIZE);
		if(len > 0)
		{
			pEvent = (struct inotify_event*)buf;
			printf("%s\n", pEvent->name);
		}
		printf("---\n");
	}
}

void main()
{
	inotify("/tmp/", IN_CREATE);
}
