#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXLINE 80
int port = 8000;

struct

class CRemoteClient
{
private:
	int m_remote_server_sock_fd;
	//CRemoteClient();
	~CRemoteClient();

public:
	int connect_remote_server(string remote_ip, int port);
	int read_remote_server();
	int write_remote_server();
	int read_pool();
	int write_pool();
	int run(); //to create read, write thread
};

CRemoteClient::~CRemoteClient();
{
	close(m_remote_server_sock_fd);
}

int CRemoteClient::connect_remote_server(string remote_ip, int port)
{
  struct sockaddr pin={0};
  char buf[MAXLINE];
  char str[MAXLINE];
  int n;


  bzero(&pin, sizeof(pin));
  pin.sin_family = AF_INET;
  inet_pton(AF_INET, remote_ip.c_str(), &pin.sin_addr);
  pin.sin_port = htons(port);
  
  m_remote_server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  n=connect(m_remote_server_sock_fd, &pin, sizeof(pin));
  if (-1 == n)
  {
     perror("call connect");
     return 1;
  }

  cout<<"connect remote %"<<remote_ip<<":"<<port<<endl;
  return 0;
}

int CRemoteClient::read_remote_server()
{
	char buf[MAXLINE];
	int n=read(m_remote_server_sock_fd, buf, MAXLINE);
	if (0 == n)
		printf("the othere side has been closed.\n");
	else
		printf("receive from server:%s\n",buf);
}

int CRemoteClient::write_remote_server()
{
	char buf[MAXLINE];
	int n = write(m_remote_server_sock_fd, buf, strlen(buf)+1);
}

int CRemoteClient::run()
{
}
