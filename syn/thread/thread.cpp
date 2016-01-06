#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t ntid;

int g_start = 0;
int g_i = 0;
pthread_mutex_t f_lock;
pthread_rwlock_t f_rwlock;

void *
thr_fn(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		int i = g_i;//读取
		printf("%u:%d \n", (unsigned int)tid, i++);
		g_i = i;//修改
		usleep(11);
	}

	return 0;
}

void *
thr_fn2(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		int i = g_i;
		printf("%u:%d \n", (unsigned int)tid, i++);
		g_i = i;
		usleep(21);		
	}

	return 0;
}

void *
thr_fn3(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		pthread_mutex_lock(&f_lock);
		int i = g_i;//读取
		printf("%u:%d \n", (unsigned int)tid, i++);
		g_i = i;//修改
		pthread_mutex_unlock(&f_lock);
		usleep(11);
	}

	return 0;
}

void *
thr_fn4(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		pthread_mutex_lock(&f_lock);
		int i = g_i;
		printf("%u:%d \n", (unsigned int)tid, i++);
		g_i = i;
		pthread_mutex_unlock(&f_lock);
		usleep(21);		
	}

	return 0;
}


void *
thr_fn5(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		pthread_rwlock_rdlock(&f_rwlock);
		int i = g_i;//读取
		pthread_rwlock_unlock(&f_rwlock);

		printf("%u:%d \n", (unsigned int)tid, i);
		
		pthread_rwlock_wrlock(&f_rwlock);
		i = g_i;
		i++;
		g_i = i;//修改
		pthread_rwlock_unlock(&f_rwlock);
		usleep(11);
	}

	return 0;
}

void *
thr_fn6(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	while(true)
	{
		if(g_start == 1)
			break;
	}

	int i;
	for(i=0; i<50; i++)
	{
		pthread_rwlock_rdlock(&f_rwlock);
		int i = g_i;//读取
		pthread_rwlock_unlock(&f_rwlock);

		printf("%u:%d \n", (unsigned int)tid, i);
		
		pthread_rwlock_wrlock(&f_rwlock);
		i = g_i;
		i++;
		g_i = i;//修改
		pthread_rwlock_unlock(&f_rwlock);
		usleep(21);		
	}

	return 0;
}


main()
{
	int err;
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();

	printf("无同步处理----------\n");
	g_start = 0;
	g_i = 0;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	err = pthread_create(&ntid, NULL, thr_fn2, NULL);
	sleep(2);
	g_start = 1;

	sleep(3);
	printf("使用mutex处理--------\n");
	g_start = 0;
	g_i = 0;
	pthread_mutex_init(&f_lock, NULL);
	err = pthread_create(&ntid, NULL, thr_fn3, NULL);
	err = pthread_create(&ntid, NULL, thr_fn4, NULL);
	sleep(2);
	g_start = 1;

	sleep(3);
	printf("使用读写锁处理--------\n");
	g_start = 0;
	g_i = 0;
	pthread_rwlock_init(&f_rwlock, NULL);
	err = pthread_create(&ntid, NULL, thr_fn5, NULL);
	err = pthread_create(&ntid, NULL, thr_fn6, NULL);
	sleep(2);
	g_start = 1;

/*
	sleep(3);
	printf("使用条件变量处理--------\n");
	g_start = 0;
	g_i = 0;
	pthread_mutex_init(&f_lock, NULL);
	err = pthread_create(&ntid, NULL, thr_fn3, NULL);
	err = pthread_create(&ntid, NULL, thr_fn4, NULL);
	sleep(2);
	g_start = 1;
*/
	sleep(5);
	pthread_mutex_destroy(&f_lock);
	pthread_rwlock_destroy(&f_rwlock);
	printf("process exit\n");
}
