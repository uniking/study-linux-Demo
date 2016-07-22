#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <errno.h>

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

	//prctl(PR_SET_NAME, "TEST_MYTHREAD");

	pid = getpid();
	tid = pthread_self();
	printf("new thread %s pid=%u tid=%u \n", (char*)arg, (unsigned int)pid, (unsigned int)tid);
	for(i=0; i<3000; i++)
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
	char name[16];
	snprintf(name, 16, "aa1");
	name[15]=0;
	err = pthread_setname_np(ntid, name);
	//err = pthread_setname_np(ntid, "my_thread");
	if(0 != err)
		printf("set name error %d %d\n", err, errno);
	printf("pid=%u tid=%u sonthread=%u\n", (unsigned int)pid, (unsigned int)tid, (unsigned int) ntid);

	//sleep(1000);//线程可以执行30s，进程只能执行15s，进程结束后线程也立即结束了
	void *retval;
	pthread_join(ntid,&retval);
	printf("process exit\n");
}

