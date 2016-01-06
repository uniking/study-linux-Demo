#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> 
#include <linux/fs.h>
#include <linux/uaccess.h>

static char buf[]="hello file";
static char readbuf[64];

void fileoprate()
{
	struct file* fp;
	mm_segment_t fs;
	loff_t pos;
	
	fp = filp_open("/home/sxis/workspace/kernel/fileoperate/kernelfile.txt", O_RDWR | O_CREAT, 0644);
	if(IS_ERR(fp))
	{
		printk(KERN_INFO "kernel create file error\n");
		return;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = 0;
	vfs_write(fp, buf, sizeof(buf), &pos);
	pos = 0;
	vfs_read(fp, readbuf, sizeof(readbuf), &pos);
	printk(KERN_INFO "read:%s\n", readbuf);
	
	filp_close(fp, NULL);
	set_fs(fs);
}

static int __init hello_start(void)
{
	printk(KERN_INFO "Loading hello module...\n");
	fileoprate();
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
