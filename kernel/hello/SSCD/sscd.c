#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> 
#include <linux/unistd.h>
static int g_hello_ddd = 0;

static int hello_test(void)
{
	//asm("int3");
	g_hello_ddd++;
	printk(KERN_INFO "Loading hello module...\n");
	printk(KERN_INFO "Hello world\n");
	return 0;
}
#define __NR_syscalls 389
char **my_sys_call_table;

static void printssct()
{
	int i=0;
	my_sys_call_table = 0xffffffff8161c3c0;//2.6无法直接导出sys_call_table了 cat /proc/kallsyms |grep sys_call_table
	while(i < __NR_syscalls)
	{
		printk("%d address=%llx\n", i, my_sys_call_table[i]);
		i++;
	}
}

EXPORT_SYMBOL(hello_test);
EXPORT_SYMBOL_GPL(g_hello_ddd);

static int __init hello_start(void)
{
	hello_test();
	printssct();
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
