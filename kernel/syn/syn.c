#include "cdev.h"

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
int globalmem_open(struct inode *inode, struct file *filp);
int globalmem_release(struct inode* inode, struct file *filp);

struct file_operations globalmem_fops =
{
	.owner = THIS_MODULE,
	.llseek = globalmem_llseek,
	.read = globalmem_read,
	.write = globalmem_write,
	//.ioctl = globalmem_ioctl,
	.open = globalmem_open,
	.release = globalmem_release,
};


static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;

	return ret;
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

int globalmem_open(struct inode *inode, struct file *filp)
{
	filp->private_data = globalmem_devp;
	return 0;
}

int globalmem_release(struct inode* inode, struct file *filp)
{
	return 0;
}

/*
int cdev_add(struct cdev *p, dev_t dev, unsigned count)
{
   p->dev = dev;
   p->count = count;
   return kobj_map(cdev_map, dev, count, NULL, exact_match, exact_lock, p);
}*/

/*
void cdev_init_(struct cdev *pcdev, struct file_operations *fops)
{
	memset(pcdev, 0, sizeof *pcdev);
	INIT_LIST_HEAD(&pcdev->list);
	kobject_init(&pcdev->kobj, &ktype_cdev_default);
	pcdev->ops = fops;
}

*/

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
	int err, devno = MKDEV(globalmem_major, index);
	cdev_init(&dev->cdev, &globalmem_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);

	if(err)
	{
		printk(KERN_NOTICE "Error %d adding globalmem %d", err, index);
	}
}

int globalmem_init(void)
{
	int result;
	dev_t devno = MKDEV(globalmem_major, 0);

	if(globalmem_major)
	{
		result = register_chrdev_region(devno, 1, "globalmem");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
		globalmem_major = MAJOR(devno);
	}

	if(result < 0)
		return result;

	globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
	if(!globalmem_devp)
	{
		result = - ENOMEM;
		goto fail_malloc;
	}

	memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

	globalmem_setup_cdev(globalmem_devp, 0);
	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return result;
}

void globalmem_exit(void)
{
	cdev_del(&globalmem_devp->cdev);
	kfree(globalmem_devp);
	unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}


MODULE_AUTHOR("Barry song");
MODULE_LICENSE("Dual BSD/GPL");

module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);
