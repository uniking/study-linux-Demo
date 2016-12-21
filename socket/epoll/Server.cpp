#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>

#define MAXLINE 800
int port = 8000;

int main(void)
{
	struct sockaddr_in sin;
	struct sockaddr_in pin;
	int listen_fd;
	int conn_fd;
	int sock_fd;
	int nready;
	int maxi;
	int max;
	int client[FD_SETSIZE];
	int address_size = sizeof(pin);
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i;
	int len;
	int n;
	int ret;

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listen_fd)
	{
		perror("call to socket");
		exit(1);
	}
	n = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (-1 == n)
	{
		perror("call to bind");
		exit(1);
	}
	n = listen(listen_fd, 20);
	if (-1 == n)
	{
		perror("call to listen");
		exit(1);
	}
	printf("Accepting connections...\n");

	while(1)
	{
		//select poll 检测socket状态， 在accept， read，write作检测，实现异步操作
		// accept4 支持非阻塞操作
		conn_fd = accept(listen_fd, (struct sockaddr *)&pin, (socklen_t*)&address_size);
		printf("accept one client\n");
		int epfd = epoll_create(1);
		//fcntl(epfd, F_SETFL, O_NONBLOCK);
		struct epoll_event event={0};
		//event.events  = EPOLLIN;
		event.events = EPOLLET | EPOLLIN;
		event.data.fd = conn_fd;
		fcntl(conn_fd, F_SETFL, O_NONBLOCK);
		epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &event);

		n = fork();
		if (-1 == n)
		{
			perror("call to fork");
			exit(1);
		}
		else if (0 == n)
		{
			close(listen_fd);
			while(1)
			{
				memset(buf,'\0',MAXLINE);
				//select poll
				while(true)
				{
					int r = epoll_wait(epfd, &event, 20, 10000);
					if(r > 0)
					{
						printf("epoll wait return %d\n", r);
						break;
					}
					else if(r == 0)
						printf("epoll wait time out, I want to do some other thing!!\n");
					else
						printf("epoll wait error!!\n");
				}

				readagain:
				printf("to read\n");
				std::string rs;
				while( (ret = read(conn_fd,buf,1)) >= 1)
					rs += std::string(buf, 1);
				printf("read ok\n");
				printf("I read %d Byte!\n",rs.size());

				writeagain:
				ret = write(conn_fd, rs.c_str(), rs.size()+1);
				printf("I write %d Byte!\n",ret);

				if (-1 == ret)
				{
					if (errno == EINTR)
					{
						goto writeagain;
					}
					else
					{
						perror("call to write!");
						break;
					}
				}
			}// end while


			ret = close(conn_fd);
			if (-1 == ret)
			{
				perror("call close");
				return -1;
			}
			exit(0);
		}
	}
	return 0;
}
