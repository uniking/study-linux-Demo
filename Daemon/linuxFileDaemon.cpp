#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void exitInfor(int state)
{
	exit(0);
	printf("exit\n");
}

static void
sig_usr(int signo)
{
	if(signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if(signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else if(signo == SIGTERM)
		printf("received SIGTERM\n");
	else
		printf("received signel %d\n", signo);
}

void init_daemon(void)
{
	int pid;
	int i;
	if(pid=fork())
		exitInfor(0);//是父进程，结束父进程
	else if(pid< 0)
		exitInfor(1);//fork失败，退出

	//是第一子进程，后台继续执行
	setsid();//第一子进程成为新的会话组长和进程组长
	//并与控制终端分离
	if(pid=fork())
		exitInfor(0);//是第一子进程，结束第一子进程
	else if(pid< 0)
		exitInfor(1);//fork失败，退出
	//是第二子进程，继续
	//第二子进程不再是会话组长

	for(i=0;i< NOFILE;++i)//关闭打开的文件描述符
		close(i);
	chdir("/tmp");//改变工作目录到/tmp
	umask(0);//重设文件创建掩模
	return;
}

void init_daemon(void);//守护进程初始化函数

main()
{
	FILE *fp;
	time_t t;
	init_daemon();//初始化为Daemon

	//接受消息
	if(signal(SIGUSR1, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if(signal(SIGUSR2, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR2\n");
	if(signal(SIGTERM, sig_usr) == SIG_ERR)
		printf("can't catch SIGTERM\n");//本sig默认是终止

	while(1)//每隔一分钟向test.log报告运行状态
	{
		sleep(60);//睡眠一分钟
		if((fp=fopen("test.log","a")) >=0)
		{
			t=time(0);
			fprintf(fp,"Im here at %s/n",asctime(localtime(&t)) );
			printf("I'm daemon\n");
			fclose(fp);
		}
	}
} 
