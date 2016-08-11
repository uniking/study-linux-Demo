

//DNS header structure
typedef struct _DNS_HEADER
{
	unsigned short id; // identification number

	unsigned char rd :1; // recursion desired
	unsigned char tc :1; // truncated message
	unsigned char aa :1; // authoritive answer
	unsigned char opcode :4; // purpose of message
	unsigned char qr :1; // query/response flag

	unsigned char rcode :4; // response code
	unsigned char cd :1; // checking disabled
	unsigned char ad :1; // authenticated data
	unsigned char z :1; // its z! reserved
	unsigned char ra :1; // recursion available

	unsigned short q_count; // number of question entries
	unsigned short ans_count; // number of answer entries
	unsigned short auth_count; // number of authority entries
	unsigned short add_count; // number of resource entries
}DNS_HEADER, *PDNS_HEADER;

typedef struct _UDPHeader
{
       u_int16_t               sourcePort;           // 16位源端口号   | Source port
       u_int16_t               destinationPort;      // 16位目的端口号 | Destination port     
       u_int16_t               len;                  // 16位封包长度   | Sequence Number
       u_int16_t               checksum;             // 16位校验和     | Acknowledgement number
} UDPHeader, *PUDPHeader;

struct ip_header
{
    #ifdef WORDS_BIGENDIAN
        u_int8_t ip_version: 4,  /* IP协议版本 */
        ip_header_length: 4; /* IP协议首部长度 */
    #else
        u_int8_t ip_header_length: 4, ip_version: 4;
    #endif
    u_int8_t ip_tos;
    /* TOS服务质量 */
    u_int16_t ip_length;
    /* 总长度 */
    u_int16_t ip_id;
    /* 标识 */
    u_int16_t ip_off;
    /* 偏移 */
    u_int8_t ip_ttl;
    /* 生存时间 */
    u_int8_t ip_protocol;
    /* 协议类型 */
    u_int16_t ip_checksum;
    /* 校验和 */
    struct in_addr ip_souce_address;
    /* 源IP地址 */
    struct in_addr ip_destination_address;
    /* 目的IP地址 */
};

typedef u_int tcp_seq;

struct tcp_header  
{  
  u_short th_sport;     /* source port */  
  u_short th_dport;     /* destination port */  
  tcp_seq th_seq;       /* sequence number */  
  tcp_seq th_ack;       /* acknowledgement number */  
  u_char th_offx2;      /* data offset, rsvd */  
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)  
  u_char th_flags;  
#define TH_FIN  0x01  
#define TH_SYN  0x02  
#define TH_RST  0x04  
#define TH_PUSH 0x08  
#define TH_ACK  0x10  
#define TH_URG  0x20  
#define TH_ECE  0x40  
#define TH_CWR  0x80  
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)  
  u_short th_win;       /* window */  
  u_short th_sum;       /* checksum */  
  u_short th_urp;       /* urgent pointer */  
};
