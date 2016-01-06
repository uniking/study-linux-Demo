#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <time.h>

#define DEV_1 "p3p1"
/*
the printer running when packet have captured
*/
void printer()
{
	printf("A packet is captured!\n");
	return;
}

/* 
just print a count every time we have a packet… 
*/
void my_callback(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char* packet)
{
	static int count = 1;
	fprintf(stdout,"%d, ",count);
	fflush(stdout);
	count++;
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
		fprintf(stderr,"Error setting filter\n");
		exit(1);
	}

	/* … and loop */
	pcap_loop(descr,-1,my_callback,NULL);

	return 0;
}
