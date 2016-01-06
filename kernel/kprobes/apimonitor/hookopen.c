   /* Trace do_execv.  Taken basically from Documentation/kprobes.txt */
   #include <linux/kernel.h>
   #include <linux/module.h>
   #include <linux/sched.h>
   #include <linux/kprobes.h>
   #include <linux/kallsyms.h>
   

static int my_do_execve(char * filename,
                  char __user *__user *argv,
                  char __user *__user *envp,
                  struct pt_regs * regs)
{
	printk("do_execve for %s from %s\n", filename, current->comm);

	jprobe_return();
	return 0;
}

long my_do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
{
	if(0 == strnicmp(fileanme, "/dev/", 5))
		printk("do_sys_open filename=%s flags=%x mode=%x\n", filename, flags, mode);

	jprobe_return();
	return 0;
}

static int my_register_blkdev(unsigned int major, const char *name)
{
	printk("register_blkdev major=%d name=%s", major, name);	

	jprobe_return();
	return 0;
}

static int my_dm_hash_insert(const char *name, const char *uuid, struct mapped_device *md)
{
	printk("register_blkdev name=%s", name);	

	jprobe_return();
	return 0;
}


static struct jprobe jprobe_do_execve = 
{
	.entry = (kprobe_opcode_t *) my_do_execve
};
static struct jprobe jprobe_do_sys_open = 
{
	.entry = (kprobe_opcode_t *) my_do_sys_open
};
static struct jprobe jprobe_register_blkdev = 
{
	.entry = (kprobe_opcode_t *) my_register_blkdev
};
static struct jprobe jprobe_dm_hash_insert = 
{
	.entry = (kprobe_opcode_t *) my_dm_hash_insert
};

#define MY_JPROBES_NUM 3

static struct jprobe *my_jprobes[MY_JPROBES_NUM] =
{
	&jprobe_do_sys_open,
	&jprobe_register_blkdev,
	&jprobe_dm_hash_insert,
};

int Init_do_sys_open()
{
	jprobe_do_sys_open.kp.addr = (kprobe_opcode_t *) kallsyms_lookup_name("do_sys_open");
	if (!jprobe_do_sys_open.kp.addr)
	{
		printk("Couldn't find %s to plant jprobe\n", "do_execve");
		return -1;
	}

	return 0;
}

int Init_register_blkdev()
{
	jprobe_register_blkdev.kp.addr = (kprobe_opcode_t *) kallsyms_lookup_name("register_blkdev");
	if (!jprobe_register_blkdev.kp.addr)
	{
		printk("Couldn't find %s to plant jprobe\n", "register_blkdev");
		return -1;
	}

	return 0;
}

int Init_dm_hash_insert()
{
	jprobe_dm_hash_insert.kp.addr = (kprobe_opcode_t *) kallsyms_lookup_name("dm_hash_insert");
	if (!jprobe_dm_hash_insert.kp.addr)
	{
		printk("Couldn't find %s to plant jprobe\n", "dm_hash_insert");
		return -1;
	}

	return 0;
}
  
int init_module(void)
{
	int ret;
	
	if(-1 == Init_do_sys_open())
		return -1;
	if(-1 == Init_register_blkdev())
		return -1;
	if(-1 == Init_dm_hash_insert())
		return -1;


	if ((ret = register_jprobes(my_jprobes, MY_JPROBES_NUM)) <0) 
	{
		printk("register_jprobe failed, returned %d\n", ret);
		return -1;
	}

	return 0;
}
  
void cleanup_module(void)
{
	unregister_jprobes(my_jprobes, MY_JPROBES_NUM);
	printk("jprobes unregistered\n");
}
  
MODULE_LICENSE("GPL");
