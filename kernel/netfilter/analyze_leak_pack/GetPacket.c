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

MODULE_LICENSE("GPL");
#define NIPQUAD(addr) \
((unsigned char *)&addr)[0], \
((unsigned char *)&addr)[1], \
((unsigned char *)&addr)[2], \
((unsigned char *)&addr)[3]

//net to host
__be16 n2h_16(__be16 integer)
{
	__be16 rInt = 0;
	((unsigned char *)&rInt)[0] = ((unsigned char *)&integer)[1];
	((unsigned char *)&rInt)[1] = ((unsigned char *)&integer)[0];

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
		struct sk_buff *sb = NULL;
		sb = skb;
		iph  = ip_hdr(sb);
		sip = iph->saddr;
		dip = iph->daddr;
		if(cap_two_ip(sip, dip))
			printk("id:%x s: %d.%d.%d.%d d: %d.%d.%d.%d protocol:%d\n", n2h_16(iph->id), NIPQUAD(sip), NIPQUAD(dip),iph->protocol);
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

static int __init sample_init(void) {
	g_filter_sip = create_ip(192,168,220,207);
	g_filter_dip = create_ip(10,0,2,15);
	nf_register_hook(&sample_ops);
	return 0;
}


static void __exit sample_exit(void) {
	nf_unregister_hook(&sample_ops);
}

module_init(sample_init);
module_exit(sample_exit);
MODULE_AUTHOR("wzl");
MODULE_DESCRIPTION("sample");
