#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
	pid_t pid = fork();
	if(pid == 0)
	{
		int c,flags;
		mqd_t mqd;
		char cc[]="hello, posix msg";
		flags = O_RDWR | O_CREAT;//O_NONBLOCK
		struct mq_attr attr;
		attr.mq_flags=flags;
		attr.mq_maxmsg=20;
		attr.mq_msgsize=2048;
		mqd = mq_open("/myname", flags, 0666, &attr);
		if(mqd<0)
		{
			printf("mq_open error\n");
			return 0;
		}

		sleep(5);//看看接收端是否阻塞
		int i = mq_send(mqd, cc, strlen(cc)+1, 1);
		if(i == 0)
			printf("send ok\n");
		sleep(2);
		mq_close(mqd);
		//mq_unlink("/myname");
	}
	else
	{
		int c,flags;
		mqd_t mqd;
		char cc[]="hello, posix msg";
		flags = O_RDWR | O_CREAT ;//O_NONBLOCK
		struct mq_attr attr;
		attr.mq_flags=flags;
		attr.mq_maxmsg=20;
		attr.mq_msgsize=2048;
		mqd = mq_open("/myname", flags, 0666, &attr);
		if(mqd<0)
		{
			printf("mq_open error\n");
			return 0;
		}

		char buf[1024]={0};
		//struct mq_attr attr;
		//mq_getattr(mqd, &attr);
		printf("to receive data\n");
		int j = mq_receive(mqd, buf, attr.mq_msgsize, NULL);
		if(j>0)
			printf("receive:%s\n", buf);
		mq_close(mqd);
	}

	return 0;
}
