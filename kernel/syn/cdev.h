#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/kobj_map.h>

#define GLOBLMEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 250

static int globalmem_major = GLOBALMEM_MAJOR;
/*
struct cdev
{
	struct kobject kobj;
	struct module *owner;
	struct file_operations *ops;
	struct list_head list;
	dev_t dev;
	unsigned int count;
};
*/

struct globalmem_dev
{
	struct cdev cdev;
	unsigned char mem[GLOBLMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;
