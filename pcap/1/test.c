#include <stdio.h>
#include <pcap.h>

/*the printer running when packet have captured*/
void printer()
{
	printf("A packet is captured!\n");
	return;
}

int main (int argc, char* argv[])
{
	/*the error code buf of libpcap*/
	char ebuf[PCAP_ERRBUF_SIZE];
	char* pNetDev = 0;

	pNetDev = pcap_lookupdev(ebuf);
	if(pNetDev == 0)
	{
		printf("pcap_lookupdev error\n");
		return -1;
	}
	else
	{
		printf("Open %s\n", pNetDev);
	}

	/*create capture handler of libpcap*/
	pcap_t *pd = pcap_open_live (pNetDev, 68, 0, 1000, ebuf);

	if(!pd)
	{
		/*start the loop of capture, loop 5 times, enter printer when capted*/
		pcap_loop (pd, 5, printer, NULL);

		pcap_close (pd);
	}
	else
	{
		printf("pcap_open_live error %s\n", ebuf);
	}


	return 0;	
}
