#include <sched.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FIBER_STACK 8192
int a;
void * stack;
int do_something(void * par)
{
	char msg[512] = {0};

	sprintf(msg, "This is son, the pid is:%d\n", getpid());
	printf(msg);

	FILE*  pNewFile  = fopen("/home/sxis/workspace/namespace/pid/1.txt", "a+");
	if(pNewFile != 0)
	{
		fwrite(msg, sizeof(char), strlen(msg), pNewFile);  
		fclose(pNewFile);
	}
	else
	{
		printf("clone thread fopen error =%d\n", errno);
	}

	free(stack); //这里我也不清楚，如果这里不释放，不知道子线程死亡后，该内存是否会释放，知情者可以告诉下,谢谢
	exit(0);
}

int main()
{
	void * stack;
	a = 1;
	stack = malloc(FIBER_STACK);//为子进程申请系统堆栈
	if(!stack)
	{
		printf("The stack failed\n");
		exit(0);
	}

	printf("creating son thread!!!\n");

	//clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VM|CLONE_VFORK, 0);//创建子线程
	//clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VFORK, 0);//创建子线程
	clone(&do_something, (char *)stack + FIBER_STACK, CLONE_NEWPID, 0);//创建子线程
	printf("This is father, my pid is: %d, the a is: %d\n", getpid(), a);
	exit(0);
}
