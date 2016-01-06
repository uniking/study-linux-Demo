#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define FIFO_NAME "/tmp/my_fifo"

int main()
{
	int err;
	pid_t pid;
	int fd[2];
	int n;
	char line[1024] = {0};
	//FIFO有名管道
	if((pid = fork()) <0)
	{
		printf("fork error\n");
	}
	else if(pid == 0)
	{
		int pipe_fd;
		mkfifo(FIFO_NAME, 0777);//创建一个有名管道
		pipe_fd = open(FIFO_NAME, O_RDWR);
		write(pipe_fd, "hello fifo\n", strlen("hello fifo\n"));
		sleep(2);//在子进程取出内容前，不能销毁fifo
		close(pipe_fd);
	}
	else
	{
		int pipe_fd;
		sleep(1);//等待父进程创建好名字管道
		pipe_fd = open(FIFO_NAME, O_RDONLY);
		n = read(pipe_fd, line, 1024);//自动阻塞
		write(STDOUT_FILENO, line, n);
		close(pipe_fd);
	}
	

	//在父进程中实验pipe
	if(pid == 0)
	{
		//pipe 1，单向 2，有公共祖先（因为没有名字，只能根据继承使用）
		if(pipe(fd) < 0)
		{
			printf("pipe error\n");
			return 0;
		}

		if((pid = fork()) <0)
		{
			printf("fork error\n");
		}
		else if(pid == 0)
		{
			close(fd[0]);
			sleep(1);
			write(fd[1], "hello pipe\n", strlen("hello pipe\n"));
		}
		else
		{
			close(fd[1]);
			n = read(fd[0], line, 1024);//自动阻塞
			write(STDOUT_FILENO, line, n);
		}

		//在父进程中实验消息队列，类似于windows的窗口消息队列
		if(pid == 0)
		{
			if(pipe(fd) < 0)
			{
				printf("pipe error\n");
				return 0;
			}

			if((pid = fork()) <0)
			{
				printf("fork error\n");
			}	
			else if(pid == 0)
			{
				int msgqueue_id;
				key_t key;
				key = ftok(".", 'm');
				msgqueue_id = msgget(key, IPC_CREAT | 0660);
				
				msgsnd(msgqueue_id, "hello", 6, 0);
				msgsnd(msgqueue_id, "world", 6, 0);
			}
			else
			{
				int msgqueue_id;
				char buf[1024] = {0};
				key_t key;
				key = ftok(".", 'm');
				sleep(1);//等待父进程发送消息
				msgqueue_id = msgget(key, IPC_CREAT | 0660);
				
				//取一条消息
				msgrcv(msgqueue_id, buf, 1024, 0, 0);
				printf("%s\n", buf);
				//再取一条消息
				msgrcv(msgqueue_id, buf, 1024, 0, 0);
				printf("%s\n", buf);
			}
			
					
		}
	}

	return 0;
}
