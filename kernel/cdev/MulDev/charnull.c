#include <linux/module.h>
#include <linux/version.h> 
#include <linux/kernel.h> 
#include <linux/types.h> 
#include <linux/kdev_t.h> 
#include <linux/fs.h> 
#include <linux/device.h> 
#include <linux/cdev.h> 

static int majornumber;
static dev_t firstdev; // Global variable for the firstdev device number 
static dev_t seconddev; // Global variable for the firstdev device number 

static struct cdev c_dev; // Global variable for the character device structure
static struct cdev c_dev2; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class 

#define DEV_NUM 2

static int my_open(struct inode *i, struct file *f) 
{
	printk(KERN_INFO "Driver: open()\n");
	return 0;
} 

static int my_close(struct inode *i, struct file *f) 
{ 
	printk(KERN_INFO "Driver: close()\n"); 
	return 0; 
} 

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) 
{
	printk(KERN_INFO "Driver: read()\n"); 
	return 0; 
} 

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off) 
{ 
	printk(KERN_INFO "Driver: write()\n"); 
	return len; 
} 

static struct file_operations pugs_fops = 
{ 
	.owner = THIS_MODULE, 
	.open = my_open, 
	.release = my_close, 
	.read = my_read, 
	.write = my_write 
}; 

static int __init ofcd_init(void) /* Constructor */ 
{ 
	int ret; 
	struct device *dev_ret; 
	printk(KERN_INFO "Namaskar: ofcd registered"); 
	if ((ret = alloc_chrdev_region(&firstdev, 0, DEV_NUM, "Shweta")) < 0) 
	{ 
		return ret; 
	} 

	majornumber = MAJOR(firstdev);
	
	if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv"))) 
	{ 
		unregister_chrdev_region(firstdev, DEV_NUM); 
		return PTR_ERR(cl); 
	}

	
	
	if (IS_ERR(dev_ret = device_create(cl, NULL, firstdev, NULL, "mynull"))) 
	{ 
		class_destroy(cl); 
		unregister_chrdev_region(firstdev, DEV_NUM); 
		return PTR_ERR(dev_ret); 
	} 

	seconddev = MKDEV(majornumber, 1);
	if (IS_ERR(dev_ret = device_create(cl, NULL, seconddev, NULL, "mynull2"))) 
	{ 
		class_destroy(cl); 
		unregister_chrdev_region(firstdev, DEV_NUM); 
		return PTR_ERR(dev_ret); 
	}


	//创建两个设备
	cdev_init(&c_dev, &pugs_fops); 
	if ((ret = cdev_add(&c_dev, firstdev, 1)) < 0) 
	{ 
		//device_destroy(cl, firstdev); 
		//class_destroy(cl); 
		unregister_chrdev_region(firstdev, DEV_NUM); 
		return ret; 
	}

	cdev_init(&c_dev2, &pugs_fops); 
	if ((ret = cdev_add(&c_dev2, seconddev, 1)) < 0) 
	{ 
		//device_destroy(cl, firstdev); 
		//class_destroy(cl); 
		unregister_chrdev_region(firstdev, DEV_NUM); 
		return ret; 
	}
	
	return 0; 
} 

static void __exit ofcd_exit(void) /* Destructor */ 
{ 
	cdev_del(&c_dev);
	cdev_del(&c_dev2);

	device_destroy(cl, firstdev);
	device_destroy(cl, seconddev); 

	class_destroy(cl); 
	unregister_chrdev_region(firstdev, DEV_NUM); 
	printk(KERN_INFO "Alvida: ofcd unregistered"); 
} 

module_init(ofcd_init); 
module_exit(ofcd_exit); 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>"); 
MODULE_DESCRIPTION("Our firstdev Character Driver");
