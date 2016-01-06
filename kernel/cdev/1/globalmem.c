#include "cdev.h"

struct globalmem_dev *globalmem_devp=0;

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
//static int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

int globalmem_open(struct inode *inode, struct file *filp);
int globalmem_release(struct inode* inode, struct file *filp);

struct file_operations globalmem_fops =
{
	.owner = THIS_MODULE,
	.llseek = globalmem_llseek,
	.read = globalmem_read,
	.write = globalmem_write,
	.open = globalmem_open,
	.release = globalmem_release,
	//.ioctl = globalmem_ioctl, //新内核已经删除.ioctl
	.unlocked_ioctl = globalmem_ioctl,
};

void debugging(char* section)
{
	struct file* fp;
	mm_segment_t fs;
	loff_t pos;
	char address[128] = {0};
	
	fp = filp_open(section, O_RDWR, 0644);
	if(IS_ERR(fp))
	{
		printk(KERN_INFO "kernel create file error\n");
		return;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = 0;
	asm("int3");//中断，获取 .text地址
	vfs_read(fp, address, 64, &pos);
	filp_close(fp, NULL);
	set_fs(fs);
}


static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;

	return ret;
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct globalmem_dev *mem_devp = filp->private_data;
	int readlen = 0;
	if(count <= GLOBLMEM_SIZE)
	{
		copy_to_user(buf, mem_devp->mem, count);
		*f_pos = *f_pos + count;
		readlen = count;
	}
	else
	{
		copy_to_user(buf, mem_devp->mem, GLOBLMEM_SIZE);
		*f_pos = *f_pos + GLOBLMEM_SIZE;
		readlen = GLOBLMEM_SIZE;
	}
	return readlen;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	//printk(KERN_INFO "globalmem_write: globalmem_devp=%x\n",globalmem_devp);
	struct globalmem_dev *mem_devp = filp->private_data;
	int writelen = 0;
	if(count <= GLOBLMEM_SIZE)
	{
		copy_from_user(mem_devp->mem, buf, count);
		//globalmem_devp->mem[count]=0; //不能这样直接操作，现在在用户空间内
		
		//
		*f_pos = *f_pos+count;//设置现在的偏移
		writelen = count;
	}
	return writelen;
}

static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

int globalmem_open(struct inode *inode, struct file *filp)
{
	int a=7;
	int b=8;
	a*=b;//测试
	printk(KERN_INFO "globalmem_open: a=%d\n",a);
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
	dev_t devno;

	//debugging("/sys/module/globalmem/sections/.text");
	devno = MKDEV(globalmem_major, 0);

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
	printk(KERN_INFO "globalmem: globalmem_devp address=%x\n", globalmem_devp);

	//asm("int3");
	memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

	globalmem_setup_cdev(globalmem_devp, 0);
	//printk(KERN_INFO "globalmem: globalmem_read address=%x\n", globalmem_read);
	printk(KERN_INFO "globalmem: return globalmem_devp address=%x\n", globalmem_devp);
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
