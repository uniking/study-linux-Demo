#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

/*
linux 内核中没有实现pthread更能，所以内核中也没有pthead.h文件，
应用层库实现，链接时需要使用 -lpthread  (pthread posix thread)
pthread_create调用内核函数clone
*/

pthread_t ntid;

void *
thr_fn(void* arg)
{
	pid_t pid;
	pthread_t tid;
	int i;

	pid = getpid();
	tid = pthread_self();
	pid_t tid2 = syscall(SYS_gettid);
	printf("new thread %s pid=%u tid=%u tid2=%u\n", arg, (unsigned int)pid, (unsigned int)tid, tid2);
	for(i=0; i<30; i++)
	{
		sleep(1);
		printf("%d\n", i);
	}
	printf("thread exit\n");
	return 0;
}

main()
{
	int err;
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	printf("pid=%u tid=%u sonthread=%u\n", (unsigned int)pid, (unsigned int)tid, (unsigned int) ntid);

	sleep(15);//线程可以执行30s，进程只能执行15s，进程结束后线程也立即结束了
	printf("process exit\n");
}
