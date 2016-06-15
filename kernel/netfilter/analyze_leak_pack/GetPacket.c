//unbuntu3.2 内核通过
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "Queue.h"

MODULE_LICENSE("GPL");
#define NIPQUAD(addr) \
((unsigned char *)&addr)[0], \
((unsigned char *)&addr)[1], \
((unsigned char *)&addr)[2], \
((unsigned char *)&addr)[3]

#define SOCKET_OPS_BASE 128
#define SOCKET_OPS_SET (SOCKET_OPS_BASE)
#define SOCKET_OPS_GET (SOCKET_OPS_BASE)
#define SOCKET_OPS_MAX (SOCKET_OPS_BASE + 1)

//net to host
__be16 n2h_16(__be16 integer)
{
	__be16 rInt = 0;
	((unsigned char *)&rInt)[0] = ((unsigned char *)&integer)[1];
	((unsigned char *)&rInt)[1] = ((unsigned char *)&integer)[0];

	return rInt;
}

__be32 n2h_32(__be32 integer)
{
	__be32 rInt = 0;
	((unsigned char *)&rInt)[0] = ((unsigned char *)&integer)[3];
	((unsigned char *)&rInt)[1] = ((unsigned char *)&integer)[2];
	((unsigned char *)&rInt)[2] = ((unsigned char *)&integer)[1];
	((unsigned char *)&rInt)[3] = ((unsigned char *)&integer)[0];
	return rInt;
}

// create_ip(127, 0, 0, 1)
__be32 create_ip(unsigned char one, unsigned char two, unsigned char three, unsigned char four)
{
	__be32 ip=0;
	((unsigned char *)&ip)[0]=one;
	((unsigned char *)&ip)[1]=two;
	((unsigned char *)&ip)[2]=three;
	((unsigned char *)&ip)[3]=four;

	return ip;
}

__be32 g_filter_sip=0;
__be32 g_filter_dip=0;

bool cap_ip(__be32 ip)
{
	if(ip == g_filter_sip)
		return true;
	return false;
}

bool cap_two_ip(__be32 s_ip, __be32 d_ip)
{
	
	if(s_ip == g_filter_sip && d_ip == g_filter_dip)
		return true;
	return false;
}

static unsigned int sample
(
	unsigned int hooknum,
	struct sk_buff * skb,
	const struct net_device *in,
	const struct net_device *out,
	int (*okfn) (struct sk_buff *)
)
{
	__be32 sip,dip;
	
	if(skb)
	{
		struct iphdr *iph;
		struct tcphdr *tcph;
		struct sk_buff *sb = NULL;
		sb = skb;
		iph  = ip_hdr(sb);
		sip = iph->saddr;
		dip = iph->daddr;
		//if(cap_two_ip(sip, dip))
		//	printk("id:%x s: %d.%d.%d.%d d: %d.%d.%d.%d protocol:%d\n", n2h_16(iph->id), NIPQUAD(sip), NIPQUAD(dip),iph->protocol);
		printk("sample\n");
		if(iph->protocol == 6)
		{
			TCP_DATA tcp_data = {0};
			//__asm__("int3");
			tcph = tcp_hdr(sb);
			//printk("%x ", n2h_32(tcph->seq));
			tcp_data.sip = sip;
			tcp_data.dip = dip;
			tcp_data.s_port = n2h_16(tcph->source);
			tcp_data.d_port = n2h_16(tcph->dest);
			tcp_data.seq = tcp_data.seq + n2h_16(iph->tot_len)*4 - ;
			tcp_data.want_seq = n2h_32(tcph->ack_seq);
			if(put_data(&tcp_data) == QUEUE_FULL)
				printk("tcp queue full\n");
		}
		
	}
	return NF_ACCEPT;
}

struct nf_hook_ops sample_ops = {
	.list =  {NULL,NULL}, //链表成员
	.hook = (nf_hookfn*)sample, //钩子函数
	.pf = PF_INET, //协议簇，对ipv4而言是PF_INET
	.hooknum = NF_INET_PRE_ROUTING, //我们称为hook点 (hook的类型)
	.priority = NF_IP_PRI_FILTER+2 //优先级
};

/////////////////////////////////////////////////////
static int recv_msg(struct sock* sk, int cmd, void __user* user, unsigned int len)
{
	int ret_len;
	printk("recv_msg\n");
	TCP_DATA tcp_data = {0};
	if(cmd == SOCKET_OPS_SET && len == sizeof(TCP_DATA))
	{
		ret_len = copy_from_user(&tcp_data, user, len);
	}

	return 0;
}

static int send_msg(struct sock* sk, int cmd, void __user* user, int* len)
{
	int ret = 0;
	int ret_len;
	TCP_DATA tcp_data = {0};
	printk("send_msg\n");
	if(cmd == SOCKET_OPS_SET && *len >= sizeof(TCP_DATA))
	{
		if(QUEUE_EMPTY == get_data(&tcp_data))
		{
			printk("tcp queue empty\n");
			ret = -1;
		}
		else
		{
			ret_len = copy_to_user(user, &tcp_data, *len);
		}
	}
	else
	{
		printk("parameter error\n");
	}

	return ret;
}
static struct nf_sockopt_ops test_sockops =
{
	.pf = PF_INET,
	.set_optmin = SOCKET_OPS_SET,
	.set_optmax = SOCKET_OPS_MAX,
	.set = recv_msg,
	.get_optmin = SOCKET_OPS_GET,
	.get_optmax = SOCKET_OPS_MAX,
	.get = send_msg,
	.owner = THIS_MODULE,
};



static int __init sample_init(void) {
	int ret;
	
	printk("init tcp queue\n");
	init_tcp_queue_sx(100000);

	printk("register sockopt\n");
	ret = nf_register_sockopt(&test_sockops);
	if(ret)
		printk("nf_register_sockopt error\n");

	g_filter_sip = create_ip(192,168,220,207);
	g_filter_dip = create_ip(220,181,12,208);

	printk("register hook\n");
	ret = nf_register_hook(&sample_ops);
	if(ret)
		printk("nf_register_hook error\n");
	return 0;
}


static void __exit sample_exit(void) {
	printk("unregister hook\n");
	nf_unregister_hook(&sample_ops);
	printk("unregister sockopt\n");
	nf_unregister_sockopt(&test_sockops);
	printk("uninit tcp queue\n");
	uninit_tcp_queue_sx();
}

module_init(sample_init);
module_exit(sample_exit);
MODULE_AUTHOR("wzl");
MODULE_DESCRIPTION("sample");
