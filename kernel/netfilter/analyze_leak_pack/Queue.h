#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> 

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

int init_tcp_queue_sx(long capacity);
void uninit_tcp_queue_sx();
int put_data(PTCP_DATA tcp_data);
int get_data(PTCP_DATA tcp_data);
