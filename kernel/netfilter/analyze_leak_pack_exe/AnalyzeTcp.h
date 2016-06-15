#include <map>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/in.h>
using namespace std;

#pragma pack(1)

typedef struct _TCP_DATA
{
	unsigned int sip;
	unsigned short s_port;
	unsigned int dip;
	unsigned short d_port;
	unsigned int seq;
	unsigned int want_seq;
}TCP_DATA, *PTCP_DATA; 

#pragma pack()

#define QUEUE_OK 0
#define QUEUE_FULL 1
#define QUEUE_EMPTY 2

#define SOCKET_OPS_BASE 128
#define SOCKET_OPS_SET (SOCKET_OPS_BASE)
#define SOCKET_OPS_GET (SOCKET_OPS_BASE)
#define SOCKET_OPS_MAX (SOCKET_OPS_BASE + 1)
