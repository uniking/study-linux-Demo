   /* Trace do_execv.  Taken basically from Documentation/kprobes.txt */
   #include <linux/kernel.h>
   #include <linux/module.h>
   #include <linux/sched.h>
   #include <linux/kprobes.h>
   #include <linux/kallsyms.h>
#include <linux/syscalls.h>

static int mprotect_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	char* FilePath = (char*)regs->si;
	printk("Original return address: 0x%lx %s\n", (unsigned long)ri->ret_addr, FilePath);
/*
	if(0 == strnicmp(FilePath, "/home/sxis/tmp/def", strlen("/home/sxis/tmp/def")))
	{
		if(regs->ax == 0)
		{
			//关闭返回的句柄，并返回错误值
			sys_close(regs->ax);
			regs->ax = -1;
		}
	}
*/

	return 0;
}

static struct kretprobe mprotect_kretprobe =
{
	.handler = mprotect_ret_handler, // return probe handler
	.maxactive = NR_CPUS // max number of kretprobe instances
};


int init_module(void)
{
	mprotect_kretprobe.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("do_sys_open");
	if(0 == mprotect_kretprobe.kp.addr)
	{
		printk("addr == 0\n");
		return -1;
	}

	register_kretprobe(&mprotect_kretprobe);
	return 0;	
}

void cleanup_module(void)
{
	unregister_kretprobe(&mprotect_kretprobe);
}

MODULE_LICENSE("GPL");
