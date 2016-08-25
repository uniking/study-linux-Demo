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
#include <mysql.h>
#include "net.h"
#include <cstring>

#include <re2/re2.h>
#include <re2/stringpiece.h>

#include <string>
#include <iostream>
using namespace std;

string g_outFile;

#define DEV_1 "p3p1"

/*
------+
| time     | timestamp | NO   |     | CURRENT_TIMESTAMP | on update CURRENT_TIMESTAMP |
| sip      | char(32)  | YES  |     | NULL              |                             |
| dip      | char(32)  | YES  |     | NULL              |                             |
| hostname | char(255) | YES  |     | NULL              |                             |
+----------+-----------+------+-----+-------------------+------

create table dns(time timestamp, sip char(32), dip char(32), hostname char(255));

*/

bool insert_log_mysql(const char* sip, const char* dip, const char* hostname);

/*
the printer running when packet have captured
*/
void printer()
{
	printf("A packet is captured!\n");
	return;
}

bool analyze_tcp(struct tcp_header* tcph, string& log)
{
	return false;
}

void get_ndis_hostname(char* in, char* out)
{
	int ln=in[0];
	int indexIn=1;
	int indexOut=0;

	while(ln != 0 && indexOut < 512)
	{
		int count=0;
		while(count < ln && indexOut < 512)
		{
			out[indexOut++] = in[indexIn++];
			count++;
		}
		ln=in[indexIn++];
		if(ln != 0)
			out[indexOut++] = '.';
	}

	if(indexOut < 512)
		out[indexOut] = 0;
	else
		sprintf(out, "error");
}

bool analyze_dns(PDNS_HEADER dnsh, u_int16_t length, string& log)
{
	bool bRtn = false;
	char hostname[1024]={0};
	u_int16_t dType = *(u_int16_t*)((char*)dnsh+length-4);
	u_int16_t dClass = *(u_int16_t*)((char*)dnsh+length-2);

	if(dnsh->qr == 0 &&
		dnsh->opcode == 0 &&
		dnsh->tc == 0 &&
		dnsh->rd == 1 )
	{
		if(ntohs(dnsh->q_count) == 1)
		{
			//if(ntohs(dType) == 1 && ntohs(dClass) == 1)
			//{
				get_ndis_hostname((char*)dnsh+12, hostname);
				log += "hostname:";
				log += hostname;
				bRtn = true;
			//}
		}
		else
		{
			cout<<"dnsh->q_count != 1"<<endl;
		}
	}
	
	return bRtn;
}

bool analyze_udp(PUDPHeader udph, u_int16_t length, string& log)
{
	bool bRtn = false;
	switch(ntohs(udph->destinationPort))
	{
		case 53://dns
			bRtn = analyze_dns((PDNS_HEADER)((char*)udph+8), length - 8, log);
			break;
		default:
			;
	}
	return bRtn;
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

bool analyze_ip(struct ip_header* iph, string& log)
{
	bool bRtn = false;
	u_char* pIpAddr;
	char buf[128]={0};

	if(filter_innet_addr(iph->ip_souce_address.s_addr) && filter_innet_addr(iph->ip_destination_address.s_addr))
		return false;

	unsigned int sss = iph->ip_souce_address.s_addr;
	pIpAddr = (u_char*)&sss;
	sprintf(buf, "sip:%d.%d.%d.%d ", pIpAddr[0], pIpAddr[1], pIpAddr[2], pIpAddr[3]);
	log+=buf;

	memset(buf, 0, 128);
	sss = iph->ip_destination_address.s_addr;
	pIpAddr = (u_char*)&sss;
	sprintf(buf, "dip:%d.%d.%d.%d ", pIpAddr[0], pIpAddr[1], pIpAddr[2], pIpAddr[3]);
	log+=buf;

	switch(iph->ip_protocol)
	{
		case 1://icmp
			break;
		case 2://igmp
			break;
		case 6://tcp
			//bRtn = analyze_tcp((struct tcp_header*) ((u_char*)iph + iph->ip_header_length*4), log);
			break;
		case 17://udp
			bRtn = analyze_udp((PUDPHeader) ((u_char*)iph + iph->ip_header_length*4), ntohs(iph->ip_length)-iph->ip_header_length*4, log);
			break;
		default:
			;
	}

	return bRtn;
}

/* 
just print a count every time we have a packet… 
*/
void my_callback(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char* packet)
{
	struct ether_header *eptr;  /* net/ethernet.h */
	int eth_type;
	bool b_log = false;
	string msg;
	char buf[128] = {0};

	//sprintf(buf, "t:%d ", pkthdr->ts);
	//msg += buf;
	eptr = (struct ether_header *) packet;
	eth_type = ntohs(eptr->ether_type);
	switch(eth_type)
	{
		case 0x0800://ip
			b_log = analyze_ip( (struct ip_header*)(packet+14) , msg);
			break;
		case 0x0806://arp
			break;
		case 0x0835://rarp
			break;
		default:
			;
	}

	if(b_log)
	{
		cout<<msg<<endl;
		string sip;
		string dip;
		string hostname;

		RE2::PartialMatch(msg, "(sip:\\d+.\\d+.\\d+.\\d+)", &sip);
		RE2::PartialMatch(sip, "(\\d+.\\d+.\\d+.\\d+)", &sip);

		RE2::PartialMatch(msg, "(dip:\\d+.\\d+.\\d+.\\d+)", &dip);
		RE2::PartialMatch(dip, "(\\d+.\\d+.\\d+.\\d+)", &dip);

		RE2::PartialMatch(msg, "(hostname:.+)", &hostname);
		RE2::PartialMatch(hostname, "([^hostname:].+)", &hostname);

		if(sip.size()>32 ||
			dip.size()>32 ||
			hostname.size()>255)
			;
		else
			insert_log_mysql(sip.c_str(), dip.c_str(), hostname.c_str());
	}
	
}

MYSQL mysql;

int init_mysql()
{
	MYSQL_RES *res;

	mysql_init(&mysql);
	
	if(!mysql_real_connect(&mysql, "localhost", "root", "abcd", "DNSdata", 0, NULL, 0) )
	{
		printf("mysql_real_connect error\n");
		return 1;
	}
	
	return 0;
}

void un_init_mysql()
{
	mysql_close(&mysql);
}

bool insert_log_mysql(const char* sip, const char* dip, const char* hostname)
{
	int t=0;

	char query[1024] = {0};
	if( snprintf(query, 1024, "insert into dns(time, sip, dip, hostname) value(now(), \'%s\', \'%s\', \'%s\')", sip, dip, hostname) > 0)
	{
		t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
		if(t)
		{
			printf("mysql_real_query error\n");
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
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

	if(argc < 5)
	{
		cout<<argv[0]<<" -f p3p2 -o filename [-p \"tcp[tcpflags] & (tcp-syn|tcp-fin) != 0 and not src and dst net loopback\"]"<<endl;
		return 0;
	}

	if(init_mysql())
	{
		cout<<"init mysql error"<<endl;
		return 1;
	}

	int pi = 1;
	string face;
	string file;
	string compile;
	while(pi < argc)
	{
		string pstr = argv[pi];
		if(pstr == "-f")
			face = argv[++pi];
		else if(pstr == "-o")
			file = argv[++pi];
		else if(pstr == "-p")
			compile = argv[++pi];
		else
			pi++;
	}
	g_outFile = file;
	cout<<argv[0]<<" -f "<<face<<" -o "<<file<<endl;

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
	//descr = pcap_open_live(face.c_str(),BUFSIZ,1,-1,errbuf);  // -1 cpu 100%
	descr = pcap_open_live(face.c_str(),BUFSIZ,1,1000,errbuf);
	if(descr == NULL)
	{
		printf("pcap_open_live(): %s\n",errbuf);
		exit(1);
	}

	/* Lets try and compile the program.. non-optimized */
	if(pcap_compile(descr,&fp,compile.c_str(),0,netp) == -1)
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

	un_init_mysql();
	return 0;
}
