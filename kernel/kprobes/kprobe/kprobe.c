/* generic kprobe */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>

static char* probe_addr;

//module_param_named(probe_addr, probe_addr, ulong, S_IRUGO | S_IWUSR);

/*For each probe you need to allocate a kprobe structure*/
static struct kprobe kp;

//long do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
/*
static void dump_state(struct pt_regs *regs)
{
	print_symbol(KERN_INFO "EIP is at %s\n", regs->eip);
	printk(KERN_INFO "eax: %08lx   ebx: %08lx   ecx: %08lx   edx: %08lx\n",
	regs->eax, regs->ebx, regs->ecx, regs->edx);
	printk(KERN_INFO "esi: %08lx   edi: %08lx   ebp: %08lx   esp: %08lx\n",
	regs->esi, regs->edi, regs->ebp, regs->esp);
	printk(KERN_INFO "ds: %04x   es: %04x\n",
	regs->xds & 0xffff, regs->xes & 0xffff);
	printk(KERN_INFO "Process %s (pid: %d, threadinfo=%p task=%p)",
	current->comm, current->pid, current_thread_info(), current);
}
*/

/*kprobe pre_handler: called just before the probed instruction is executed*/
int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	char* pTmp = NULL;
	char* FilePath = (char*)regs->si;
	//查看函数传值查询，将/home/sxis/tmp/abc重定向为/home/sxis/tmp/def
	if(0 == strnicmp(FilePath, "/home/sxis/tmp/abc", strlen("/home/sxis/tmp/abc")))
	{
		printk("pre_handler: do_sys_named %s\n", FilePath);
		pTmp = strstr(FilePath, "abc");
		if(NULL != pTmp)
		{
			memcpy(pTmp, "def", 3);
			printk("abc change to def\n");
		}
		else
		{
			printk("not find abd\n");
		}
	}
	
	
	return 0;
}

/*kprobe post_handler: called after the probed instruction is executed*/
void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
	printk("post_handler: p->addr=0x%p\n", p->addr);
	//dump_state(regs);
	return;
}

/* fault_handler: this is called if an exception is generated for any
* instruction within the pre- or post-handler, or when Kprobes
* single-steps the probed instruction.
*/
int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk("fault_handler: p->addr=0x%p, trap #%dn",
	p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

int init_module(void)
{
	int ret;

	probe_addr = (char*)kallsyms_lookup_name("do_sys_open");
	if (!probe_addr)
	{
		printk("trace-generic: provide probe_addr paramter\n");
		return -1;
	}

	

	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;
	kp.addr = (kprobe_opcode_t*) probe_addr;

	printk("setting probe at address %p\n",kp.addr);

	/* register the kprobe now */
	if (!kp.addr)
	{
		printk("Couldn't find %s to plant kprobe\n", "do_fork");
		return -1;
	}
	if ((ret = register_kprobe(&kp) < 0))
	{
		printk("register_kprobe failed, returned %d\n", ret);
		return -1;
	}
	printk("kprobe registered\n");
	return 0;
}

void cleanup_module(void)
{
	unregister_kprobe(&kp);
	printk("kprobe unregistered\n");
}

MODULE_LICENSE("GPL");
