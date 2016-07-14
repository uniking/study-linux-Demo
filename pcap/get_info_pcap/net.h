
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
