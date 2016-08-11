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
/*the printer running when packet have captured*/
void printer()
{
	printf("A packet is captured!\n");
	return;
}

/* callback function that is passed to pcap_loop(..) and called each time
* a packet is recieved                                                    */
void my_callback(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char*
packet)
{
	static int count = 1;
	fprintf(stdout,"%d,",count);
	if(count == 4)
		fprintf(stdout,"Come on baby sayyy you love me!!!");
	if(count == 7)
		fprintf(stdout,"Tiiimmmeesss!!");
	fflush(stdout);
	count++;
}

int main(int argc,char **argv)
{
	int i;
	char *dev;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* descr;
	const u_char *packet;
	struct pcap_pkthdr hdr;     /* pcap.h */
	struct ether_header *eptr;  /* net/ethernet.h */

	if(argc != 2)
	{
		fprintf(stdout,"Usage: %s numpackets\n",argv[0]);
		return 0;
	}

	/* grab a device to peak into… */
	dev = pcap_lookupdev(errbuf);
	if(dev == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}
	/* open device for reading */
	//descr = pcap_open_live(dev,BUFSIZ,0,-1,errbuf);
	descr = pcap_open_live(DEV_1,BUFSIZ,0,-1,errbuf);
	if(descr == NULL)
	{
		printf("pcap_open_live(): %s\n",errbuf); exit(1);
	}

	/* allright here we call pcap_loop(..) and pass in our callback function */
	/* int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user)*/
	pcap_loop(descr,atoi(argv[1]),my_callback,NULL);

	fprintf(stdout,"\nDone processing packets… wheew!\n");
	return 0;
}
