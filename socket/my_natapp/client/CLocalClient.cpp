#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "../common/common.hpp"

#define MAXLINE 80
int port = 8000;

class CLocalClient
{
private:
	map<int, int> m_socket_tunnel;//<nat server socket, user server socket>
public:
	CLocalClient();
	int connect_local_server(int socket, string local_ip, int local_port, string remote_ip, int remote_port);
	int read_pool();
	int write_pool();

	
private:
};

int CLocalClient::connect_local_server(int socket, string local_ip, int local_port, string remote_ip, int remote_port)
{
  struct sockaddr pin={0};
  char buf[MAXLINE];
  char str[MAXLINE];
  int n;
	int sock_fd;


  bzero(&pin, sizeof(pin));
  pin.sin_family = AF_INET;
  inet_pton(AF_INET, remote_ip.c_str(), &pin.sin_addr);
  pin.sin_port = htons(port);
  
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  n=connect(sock_fd, &pin, sizeof(pin));
  if (-1 == n)
  {
     perror("call connect");
     return 1;
  }

	m_socket_tunnel.insert(make_pair(socket, socket_fd));
  cout<<"connect remote %"<<remote_ip<<":"<<port<<endl;
  return 0;
}
