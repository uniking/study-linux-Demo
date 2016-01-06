#include <linux/module.h> 
#include <linux/version.h> 
#include <linux/kernel.h> 
#include <linux/types.h> 
#include <linux/kdev_t.h> 
#include <linux/fs.h> 
#include <linux/device.h> 
#include <linux/cdev.h> 
#include <linux/uaccess.h> 
#include <asm/io.h> 
#define VRAM_BASE 0x00001000  //cat /proc/iomem 查看Video RAM的地址范围 如果没有，用System RAM试试
#define VRAM_SIZE 0x00009000 

static void __iomem *vram; 
static dev_t first; 
static struct cdev c_dev; 
static struct class *cl; 
static int my_open(struct inode *i, struct file *f) 
{ 
	return 0; 
} 

static int my_close(struct inode *i, struct file *f) 
{ 
	return 0; 
} 

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) 
{ 
	int i; 
	u8 byte; 
	if (*off >= VRAM_SIZE) 
	{
	 	return 0; 
	} 
	if (*off + len > VRAM_SIZE) 
	{ 
		len = VRAM_SIZE - *off; 
	} 
	for (i = 0; i < len; i++) 
	{ 
		byte = ioread8((u8 *)vram + *off + i); 
		if (copy_to_user(buf + i, &byte, 1)) 
		{ 
			return -EFAULT; 
		} 
	} 
	*off += len; 
	return len; 
} 

static ssize_t my_write( struct file *f, const char __user *buf, size_t len, loff_t *off) 
{ 
	int i; 
	u8 byte; 
	if (*off >= VRAM_SIZE) 
	{
		printk(KERN_ERR "*off >= VRAM_SIZE\n"); 
		return 0; 
	} 
	if (*off + len > VRAM_SIZE) 
	{ 
		len = VRAM_SIZE - *off; 
	} 

	for (i = 0; i < len; i++) 
	{ 
		if (copy_from_user(&byte, buf + i, 1)) 
		{ 
			printk(KERN_ERR "copy_from_user(&byte, buf + i, 1) error\n"); 
			return -EFAULT; 
		} 
		iowrite8(byte, (u8 *)vram + *off + i); 
	} 
	*off += len;
	printk(KERN_ERR "write video ram\n"); 
	return len; 
} 

static struct file_operations vram_fops = 
{ 
	.owner = THIS_MODULE, 
	.open = my_open, 
	.release = my_close, 
	.read = my_read, 
	.write = my_write 
}; 

static int __init vram_init(void) /* Constructor */ 
{ 
	int ret; 
	struct device *dev_ret; 
	if ((vram = ioremap(VRAM_BASE, VRAM_SIZE)) == NULL) 
	{ 
		printk(KERN_ERR "Mapping video RAM failed\n"); 
		return -ENOMEM; 
	} 
	if ((ret = alloc_chrdev_region(&first, 0, 1, "vram")) < 0) 
	{ 
		return ret; 
	} 
	if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv"))) 
	{ 
		unregister_chrdev_region(first, 1); 
		return PTR_ERR(cl); 
	} 
	if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "vram"))) 
	{ 
		class_destroy(cl); 
		unregister_chrdev_region(first, 1); 
		return PTR_ERR(dev_ret); 
	} 
	cdev_init(&c_dev, &vram_fops); 
	if ((ret = cdev_add(&c_dev, first, 1)) < 0) 
	{ 
		device_destroy(cl, first); 
		class_destroy(cl); 
		unregister_chrdev_region(first, 1); 
		return ret; 
	} 
	return 0; 
} 

static void __exit vram_exit(void) /* Destructor */ 
{ 
	cdev_del(&c_dev); 
	device_destroy(cl, first); 
	class_destroy(cl); 
	unregister_chrdev_region(first, 1); 
	iounmap(vram); 
} 

module_init(vram_init); 
module_exit(vram_exit); 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>"); 
MODULE_DESCRIPTION("Video RAM Driver");
