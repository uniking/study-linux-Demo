#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> 
static int g_hello_ddd = 0;

static int hello_test(void)
{
	//asm("int3");
	g_hello_ddd++;
	printk(KERN_INFO "Loading hello module...\n");
	printk(KERN_INFO "Hello world\n");
	return 0;
}

EXPORT_SYMBOL(hello_test);
EXPORT_SYMBOL_GPL(g_hello_ddd);

static int __init hello_start(void)
{
	hello_test();
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
