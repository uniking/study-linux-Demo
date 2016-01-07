#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <time.h>
//#include <remote-ext.h>

#define DEV_1 "p3p1"

//发送arp包
int main(int argc, char* argv[])
{
	u_char packet[100] = {0};
	pcap_if_t *alldevs = NULL;
	pcap_if_t *d = NULL;
	int i=0,j=0;
	int inum=0;
	char errbuf[PCAP_ERRBUF_SIZE]={0},error[PCAP_ERRBUF_SIZE]={0};
	pcap_t *inp=NULL,*outp=NULL;

	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):",i);
	scanf("%d", &inum);

	if(inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++)
		;

	/* Open the device */
	/* Open the adapter */

	//	inp=pcap_open_offline("f:\\ethernet",error);
	if ((outp = pcap_open_live(d->name,	// name of the device
					65536,			// portion of the packet to capture. 
									// 65536 grants that the whole packet will be captured on all the MACs.
					1,				// promiscuous mode (nonzero means promiscuous)
					1000,			// read timeout
					errbuf			// error buffer
		)) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/*broadcast */
	packet[0]=255;
	packet[1]=255;
	packet[2]=255;
	packet[3]=255;
	packet[4]=255;
	packet[5]=255;

	/* set mac source tomy mac */
	packet[6]=0x00;
	packet[7]=0x0b;
	packet[8]=0xdb;
	packet[9]=0xdd;
	packet[10]=0x3f;
	packet[11]=0xa1;
	// type = arp
	packet[12]=0x08;
	packet[13]=0x06;
	//data packet ************************************
	// hardware type =1 ethernet  (6 IEE 802)
	packet[14]=0x00;
	packet[15]=0x01;
	//protocol address type IPV4	
	packet[16]=0x08;
	packet[17]=0x00;
	//hardware address length = mac size
	packet[18]=0x06;
	// protocol address length = ipv4 length
	packet[19]=0x04;
	// opcode 1 = request , 2= reply
	packet[20]=0x00;
	packet[21]=0x01;
	//my mac
	packet[22]=0x00;
	packet[23]=0x0b;
	packet[24]=0xdb;
	packet[25]=0x5e;
	packet[26]=0x3f;
	packet[27]=0xa1;
	//my ip
	packet[28]=192;
	packet[29]=168;
	packet[30]=220;
	packet[31]=208;

	//dest mac 
	packet[32]=0;
	packet[33]=0;
	packet[34]=0;
	packet[35]=0;
	packet[36]=0;
	packet[37]=0;
	//dest ip
	packet[38]=192;
	packet[39]=168;
	packet[40]=220;
	packet[41]=208;


	/* Fill the rest of the packet */
	for(i=42;i<100;i++)
	{
		packet[i]=0;
	}

	for(i=0;i<2000000000;i++)
	{
		/* Send down the packet */
		pcap_sendpacket(outp,packet,60);
		//sleep(1);

		//pcap_sendqueue_transmit(outp, squeue, sync);
	}

	return 0;
}
