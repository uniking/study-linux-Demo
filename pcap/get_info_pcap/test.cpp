#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <time.h>
#include <errno.h>
#include "net.h"

#define DEV_1 "p3p1"
/*
the printer running when packet have captured
*/
void printer()
{
	printf("A packet is captured!\n");
	return;
}

void analyze_tcp(struct tcp_header* tcph)
{
}


bool filter_innet_addr(in_addr_t n_ip_addr)
{

	in_addr_t ip_addr = ntohl(n_ip_addr);
	//printf("%x\n", ip_addr);

	// 10.0.0.0 - 10.255.255.255
	if(ip_addr >=  0x0a000000 && ip_addr <= 0x0affffff)
		return true;

	//172.16.0.0 - 172.31.255.255
	if(ip_addr >=  0xac100000 && ip_addr <= 0xac1fffff)
		return true;

	//192.168.0.0 - 192.168.255.255
	if(ip_addr >=  0xc0a80000 && ip_addr <= 0xc0a8ffff)
		return true;

	return false;
}

void analyze_ip(struct ip_header* iph)
{
	u_char* pIpAddr;

	if(filter_innet_addr(iph->ip_souce_address.s_addr) && filter_innet_addr(iph->ip_destination_address.s_addr))
		return;

	unsigned int sss = iph->ip_souce_address.s_addr;
	pIpAddr = (u_char*)&sss;
	printf("s:%d.%d.%d.%d ", pIpAddr[0], pIpAddr[1], pIpAddr[2], pIpAddr[3]);

	sss = iph->ip_destination_address.s_addr;
	pIpAddr = (u_char*)&sss;
	printf("d:%d.%d.%d.%d ", pIpAddr[0], pIpAddr[1], pIpAddr[2], pIpAddr[3]);


	switch(iph->ip_protocol)
	{
		case 1://icmp
			break;
		case 2://igmp
			break;
		case 6://tcp
			analyze_tcp((struct tcp_header*) ((u_char*)iph) + iph->ip_header_length*4);
			break;
		case 17://udp
			break;
		default:
			;
	}

	printf("\n");
}

/* 
just print a count every time we have a packet… 
*/
void my_callback(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char* packet)
{
	struct ether_header *eptr;  /* net/ethernet.h */
	int eth_type;

	eptr = (struct ether_header *) packet;
	eth_type = ntohs(eptr->ether_type);
	switch(eth_type)
	{
		case 0x0800://ip
			analyze_ip( (struct ip_header*)(packet+14) );
			break;
		case 0x0806://arp
			break;
		case 0x0835://rarp
			break;
		default:
			;
	}

}

/*
执行 ./test "host www.baidu.com"进行测试， 
只过滤主机为www.baidu.com的数据包
*/
int main(int argc,char **argv)
{
	int i;
	char *dev;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* descr;
	const u_char *packet;
	struct pcap_pkthdr hdr;     /* pcap.h                    */
	struct ether_header *eptr;  /* net/ethernet.h            */
	struct bpf_program fp;      /* hold compiled program     */
	bpf_u_int32 maskp;          /* subnet mask               */
	bpf_u_int32 netp;           /* ip                        */

	if(argc != 2)
	{
		fprintf(stdout,"Usage: %s \"filter program\"\n",argv[0]);
		return 0;
	}

	/* grab a device to peak into… */
	dev = pcap_lookupdev(errbuf);
	if(dev == NULL)
	{
		fprintf(stderr,"%s\n",errbuf);
		exit(1);
	}

	/* ask pcap for the network address and mask of the device */
	pcap_lookupnet(dev,&netp,&maskp,errbuf);

	/* open device for reading this time lets set it in promiscuous
	* mode so we can monitor traffic to another machine             */
	//descr = pcap_open_live(dev,BUFSIZ,1,-1,errbuf);
	descr = pcap_open_live(DEV_1,BUFSIZ,1,-1,errbuf);
	if(descr == NULL)
	{
		printf("pcap_open_live(): %s\n",errbuf);
		exit(1);
	}

	/* Lets try and compile the program.. non-optimized */
	if(pcap_compile(descr,&fp,argv[1],0,netp) == -1)
	{
		fprintf(stderr,"Error calling pcap_compile\n");
		exit(1);
	}

	/* set the compiled program as the filter */
	if(pcap_setfilter(descr,&fp) == -1)
	{
		fprintf(stderr,"Error setting filter %m\n");
		exit(errno);
	}

	/* … and loop */
	pcap_loop(descr,-1,my_callback,NULL);

	return 0;
}
