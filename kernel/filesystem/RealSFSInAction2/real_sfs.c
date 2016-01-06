/* Real Simula File System Module */
#include <linux/module.h> /* For module related macros, ... */
#include <linux/kernel.h> /* For printk, ... */
#include <linux/version.h> /* For LINUX_VERSION_CODE & KERNEL_VERSION */
#include <linux/fs.h> /* For system calls, structures, ... */
#include <linux/errno.h> /* For error codes */
#include <linux/slab.h> /* For kzalloc, ... */
#include <linux/buffer_head.h> /* map_bh, block_write_begin, block_write_full_page, generic_write_end, ... */
#include <linux/mpage.h> /* mpage_readpage, ... */

#include "real_sfs_ds.h" /* For SFS related defines, data structures, ... */
#include "real_sfs_ops.h" /* For SFS related operations */

/*
 * Data declarations
 */
static struct file_system_type sfs;
static struct super_operations sfs_sops;
static struct inode_operations sfs_iops;
static struct file_operations sfs_fops;
static struct address_space_operations sfs_aops;

static struct inode *sfs_root_inode;

/*
 * File Operations
 */
static int sfs_file_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "sfs: sfs_file_release\n");
	return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
static int sfs_readdir(struct file *file, void *dirent, filldir_t filldir)
{
	struct dentry *de = file->f_dentry;
	sfs_info_t *info = de->d_inode->i_sb->s_fs_info;
	int retval;

	printk(KERN_INFO "sfs: sfs_readdir: %Ld\n", file->f_pos);

	if (file->f_pos == 0)
	{
		retval = filldir(dirent, ".", 1, file->f_pos, de->d_inode->i_ino, DT_DIR);
		if (retval)
			return retval;
		file->f_pos++;
	}
	if (file->f_pos == 1)
	{
		retval = filldir(dirent, "..", 2, file->f_pos, de->d_parent->d_inode->i_ino, DT_DIR);
		if (retval)
			return retval;
		file->f_pos++;
	}
	return sfs_list(info, file, dirent, filldir);
}
#else
static int sfs_iterate(struct file *file, struct dir_context *ctx)
{
	struct dentry *de = file->f_dentry;
	sfs_info_t *info = de->d_inode->i_sb->s_fs_info;

	printk(KERN_INFO "sfs: sfs_iterate: %Ld\n", ctx->pos);

	if (!dir_emit_dots(file, ctx))
	{
		return -ENOSPC;
	}
	return sfs_list(info, file, ctx);
}
#endif

static my_generic_file_open(struct inode* inode, struct file* filp)
{
	printk(KERN_INFO "sfs: open: %s\n", filp->f_path.dentry->d_iname);
	return generic_file_open(inode, filp);
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0))
ssize_t my_do_sync_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
  ssize_t sRtn = do_sync_read(filp, buf, len, ppos);
  printk(KERN_INFO "sfs: sync read len=%lld ppos=%lld buf=%s \n", len, ppos, buf);
  return sRtn;
}

ssize_t my_do_sync_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
	char* pBuffer = vmalloc(len+1);
	if(NULL == pBuffer)
		return 0;

	memcpy(pBuffer, buf, len);
	pBuffer[len]=0;
	printk(KERN_INFO "sfs: sync write len=%lld ppos=%lld buf=%s \n", len, ppos, pBuffer);

	vfree(pBuffer);
	return do_sync_write(filp, buf, len, ppos);
}
#else
static ssize_t my_new_sync_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
	ssize_t sRtn = new_sync_read(filp, buf, len, ppos);
	printk(KERN_INFO "sfs: sync read len=%d ppos=%x buf=%s \n", len, ppos, buf);
	return sRtn;
}

static ssize_t my_new_sync_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
	printk(KERN_INFO "sfs: sync write len=%d ppos=%x buf=%s \n", len, ppos, buf);
	return new_sync_write(filp, buf, len, ppos);
}

#endif

int my_generic_file_mmap(struct file * file,struct vm_area_struct * vma)
{
    printk(KERN_INFO "sfs: to mmap \n");
    return generic_file_mmap(file, vma);
}

static struct file_operations sfs_fops =
{
	//open: generic_file_open,
	open: my_generic_file_open,
	release: sfs_file_release,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0))
	//read: do_sync_read,
	//write: do_sync_write,
	read: my_do_sync_read,
	write: my_do_sync_write,
	aio_read: generic_file_aio_read,
	aio_write: generic_file_aio_write,
#else
	//read: new_sync_read,
	read: my_new_sync_read,
	//write: new_sync_write,
	write: my_new_sync_write,
	read_iter: generic_file_read_iter,
	write_iter: generic_file_write_iter,
#endif

	mmap: my_generic_file_mmap,

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	fsync: simple_sync_file
#else
	fsync: noop_fsync
#endif
};
static struct file_operations sfs_dops =
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
	readdir: sfs_readdir
#else
	iterate: sfs_iterate
#endif
};

static int sfs_get_block(struct inode *inode, sector_t iblock, struct buffer_head *bh_result, int create)
{
	struct super_block *sb = inode->i_sb;
	sfs_info_t *info = (sfs_info_t *)(sb->s_fs_info);
	sfs_file_entry_t fe;
	sector_t phys;
	int retval;

	printk(KERN_INFO "sfs: sfs_get_block called for I: %ld, B: %lld, C: %d\n", inode->i_ino, iblock, create);

	if (iblock >= SIMULA_FS_DATA_BLOCK_CNT)
	{
		return -ENOSPC;
	}
	if ((retval = sfs_get_file_entry(info, inode->i_ino, &fe)) < 0)
	{
		return retval;
	}
	if (!fe.blocks[iblock])
	{
		if (!create)
		{
			return -EIO;
		}
		else
		{
			if ((fe.blocks[iblock] = sfs_get_data_block(info)) == INV_BLOCK)
			{
				return -ENOSPC;
			}
			if ((retval = sfs_update_file_entry(info, inode->i_ino, &fe)) < 0)
			{
				return retval;
			}
		}
	}
	phys = fe.blocks[iblock];
	map_bh(bh_result, sb, phys);

	return 0;
}
static int sfs_readpage(struct file *file, struct page *page)
{
	printk(KERN_INFO "sfs: sfs_readpage\n");
	return mpage_readpage(page, sfs_get_block);
}
static int sfs_write_begin(struct file *file, struct address_space *mapping,
	loff_t pos, unsigned len, unsigned flags, struct page **pagep, void **fsdata)
{
	printk(KERN_INFO "sfs: sfs_write_begin\n");
	*pagep = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
	return block_write_begin(file, mapping, pos, len, flags, pagep, fsdata,
		sfs_get_block);
#else
	return block_write_begin(mapping, pos, len, flags, pagep, sfs_get_block);
#endif
}
static int sfs_writepage(struct page *page, struct writeback_control *wbc)
{
	printk(KERN_INFO "sfs: sfs_writepage\n");
	return block_write_full_page(page, sfs_get_block, wbc);
}
static struct address_space_operations sfs_aops =
{
	.readpage = sfs_readpage,
	.write_begin = sfs_write_begin,
	.writepage = sfs_writepage,
	.write_end = generic_write_end
};

/*
 * Inode Operations
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
static int sfs_inode_create(struct inode *parent_inode, struct dentry *dentry, int mode, struct nameidata *nameidata)
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
static int sfs_inode_create(struct inode *parent_inode, struct dentry *dentry, umode_t mode, struct nameidata *nameidata)
#else
static int sfs_inode_create(struct inode *parent_inode, struct dentry *dentry, umode_t mode, bool excl)
#endif
{
	char fn[dentry->d_name.len + 1];
	int perms = 0;
	sfs_info_t *info = (sfs_info_t *)(parent_inode->i_sb->s_fs_info);
	int ino;
	struct inode *file_inode;
	sfs_file_entry_t fe;

	printk(KERN_INFO "sfs: sfs_inode_create dentry=%s mode=%x \n", dentry->d_iname, mode);

	strncpy(fn, dentry->d_name.name, dentry->d_name.len);
	fn[dentry->d_name.len] = 0;
	if (mode & (S_IRUSR | S_IRGRP | S_IROTH))
		mode |= (S_IRUSR | S_IRGRP | S_IROTH);
	if (mode & (S_IWUSR | S_IWGRP | S_IWOTH))
		mode |= (S_IWUSR | S_IWGRP | S_IWOTH);
	if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
		mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
	perms |= (mode & S_IRUSR) ? 4 : 0;
	perms |= (mode & S_IWUSR) ? 2 : 0;
	perms |= (mode & S_IXUSR) ? 1 : 0;
	if ((ino = sfs_create(info, fn, perms, &fe)) == INV_INODE)
		return -ENOSPC;

	file_inode = new_inode(parent_inode->i_sb);
	if (!file_inode)
	{
		sfs_remove(info, fn); // Nothing to do, even if it fails
		return -ENOMEM;
	}
	printk(KERN_INFO "sfs: Created new VFS inode for #%d, let's fill in\n", ino);
	file_inode->i_ino = ino;
	file_inode->i_size = fe.size;
	file_inode->i_mode = S_IFREG | mode;
	file_inode->i_atime.tv_sec = file_inode->i_mtime.tv_sec = file_inode->i_ctime.tv_sec = fe.timestamp;
	file_inode->i_atime.tv_nsec = file_inode->i_mtime.tv_nsec = file_inode->i_ctime.tv_nsec = 0;
	file_inode->i_mapping->a_ops = &sfs_aops;
	file_inode->i_fop = &sfs_fops;
	if (insert_inode_locked(file_inode) < 0)
	{
		make_bad_inode(file_inode);
		iput(file_inode);
		sfs_remove(info, fn); // Nothing to do, even if it fails
		return -EIO;
	}
	d_instantiate(dentry, file_inode);
	unlock_new_inode(file_inode);

	return 0;
}
static int sfs_inode_unlink(struct inode *parent_inode, struct dentry *dentry)
{
	char fn[dentry->d_name.len + 1];
	sfs_info_t *info = (sfs_info_t *)(parent_inode->i_sb->s_fs_info);
	int ino;
	struct inode *file_inode = dentry->d_inode;

	printk(KERN_INFO "sfs: sfs_inode_unlink\n");

	strncpy(fn, dentry->d_name.name, dentry->d_name.len);
	fn[dentry->d_name.len] = 0;
	if ((ino = sfs_remove(info, fn)) == INV_INODE)
		return -EINVAL;

	inode_dec_link_count(file_inode);
	return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
static struct dentry *sfs_inode_lookup(struct inode *parent_inode, struct dentry *dentry, struct nameidata *nameidata)
#else
static struct dentry *sfs_inode_lookup(struct inode *parent_inode, struct dentry *dentry, unsigned int flags)
#endif
{
	sfs_info_t *info = (sfs_info_t *)(parent_inode->i_sb->s_fs_info);
	char fn[dentry->d_name.len + 1];
	int ino;
	sfs_file_entry_t fe;
	struct inode *file_inode = NULL;

	printk(KERN_INFO "sfs: sfs_inode_lookup\n");

	if (parent_inode->i_ino != sfs_root_inode->i_ino)
		return ERR_PTR(-ENOENT);
	strncpy(fn, dentry->d_name.name, dentry->d_name.len);
	fn[dentry->d_name.len] = 0;
	if ((ino = sfs_lookup(info, fn, &fe)) == INV_INODE)
	  return d_splice_alias(file_inode, dentry); // Possibly create a new one

	printk(KERN_INFO "sfs: Getting an existing inode\n");
	file_inode = iget_locked(parent_inode->i_sb, ino);
	if (!file_inode)
		return ERR_PTR(-EACCES);
	if (file_inode->i_state & I_NEW)
	{
		printk(KERN_INFO "sfs: Got new VFS inode for #%d, let's fill in\n", ino);
		file_inode->i_size = fe.size;
		file_inode->i_mode = S_IFREG;
		file_inode->i_mode |= ((fe.perms & 4) ? S_IRUSR | S_IRGRP | S_IROTH : 0);
		file_inode->i_mode |= ((fe.perms & 2) ? S_IWUSR | S_IWGRP | S_IWOTH : 0);
		file_inode->i_mode |= ((fe.perms & 1) ? S_IXUSR | S_IXGRP | S_IXOTH : 0);
		file_inode->i_atime.tv_sec = file_inode->i_mtime.tv_sec = file_inode->i_ctime.tv_sec = fe.timestamp;
		file_inode->i_atime.tv_nsec = file_inode->i_mtime.tv_nsec = file_inode->i_ctime.tv_nsec = 0;
		file_inode->i_mapping->a_ops = &sfs_aops;
		file_inode->i_fop = &sfs_fops;
		unlock_new_inode(file_inode);
	}
	else
	{
		printk(KERN_INFO "sfs: Got VFS inode from inode cache\n");
	}
	d_add(dentry, file_inode);
	return NULL;
	// Above 2 lines can be replaced by 'return d_splice_alias(file_inode, dentry);'
}
static struct inode_operations sfs_iops =
{
	create: sfs_inode_create,
	unlink: sfs_inode_unlink,
	lookup: sfs_inode_lookup
};

/*
 * Super-Block Operations
 */
static void sfs_put_super(struct super_block *sb)
{
	sfs_info_t *info = (sfs_info_t *)(sb->s_fs_info);

	printk(KERN_INFO "sfs: sfs_put_super\n");
	if (info)
	{
		shut_browsing(info);
		kfree(info);
		sb->s_fs_info = NULL;
	}
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,34))
static int sfs_write_inode(struct inode *inode, int do_sync)
#else
static int sfs_write_inode(struct inode *inode, struct writeback_control *wbc)
#endif
{
	sfs_info_t *info = (sfs_info_t *)(inode->i_sb->s_fs_info);
	int size, timestamp, perms;

	printk(KERN_INFO "sfs: sfs_sops sfs_write_inode (i_ino = %ld)\n", inode->i_ino);

	if (!(S_ISREG(inode->i_mode))) // Real SFS deals only with regular files
		return 0;

	size = i_size_read(inode);
	timestamp = inode->i_mtime.tv_sec > inode->i_ctime.tv_sec ? inode->i_mtime.tv_sec : inode->i_ctime.tv_sec;
	perms = 0;
	perms |= (inode->i_mode & (S_IRUSR | S_IRGRP | S_IROTH)) ? 4 : 0;
	perms |= (inode->i_mode & (S_IWUSR | S_IWGRP | S_IWOTH)) ? 2 : 0;
	perms |= (inode->i_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) ? 1 : 0;

	printk(KERN_INFO "sfs: sfs_sops sfs_write_inode with %d bytes @ %d secs w/ %o\n",
		size, timestamp, perms);

	return sfs_update(info, inode->i_ino, &size, &timestamp, &perms);
}

static struct super_operations sfs_sops =
{
	put_super: sfs_put_super,
	//statfs: sfs_statfs, /* implement for df to show it up */
	write_inode: sfs_write_inode
};

/*
 * File-System Supporting Operations
 */
static int get_bit_pos(unsigned int val)
{
	int i;

	for (i = 0; val; i++)
	{   
		val >>= 1;
	}   
	return (i - 1); 
}
static int sfs_fill_super(struct super_block *sb, void *data, int silent)
{
	sfs_info_t *info;

	printk(KERN_INFO "sfs: sfs_fill_super\n");
	if (!(info = (sfs_info_t *)(kzalloc(sizeof(sfs_info_t), GFP_KERNEL))))
		return -ENOMEM;
	info->vfs_sb = sb;
	if (init_browsing(info) < 0)
	{
		kfree(info);
		return -EIO;
	}
	/* Updating the VFS super_block */
	sb->s_magic = info->sb.type;
	sb->s_blocksize = info->sb.block_size;
	sb->s_blocksize_bits = get_bit_pos(info->sb.block_size);
	sb->s_type = &sfs; // file_system_type
	sb->s_op = &sfs_sops; // super block operations

	sfs_root_inode = iget_locked(sb, ROOT_INODE_NUM); // obtain an inode from VFS
	if (!sfs_root_inode)
	{
		shut_browsing(info);
		kfree(info);
		return -EACCES;
	}
	if (sfs_root_inode->i_state & I_NEW) // allocated fresh now
	{
		printk(KERN_INFO "sfs: Got root's new VFS inode, let's fill in\n");
		sfs_root_inode->i_op = &sfs_iops; // inode operations
		sfs_root_inode->i_mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO;
		sfs_root_inode->i_fop = &sfs_dops; // file operations for directory
		sfs_root_inode->i_mapping->a_ops = &sfs_aops; // address operations
		unlock_new_inode(sfs_root_inode);
	}
	else
	{
		printk(KERN_INFO "sfs: Got root's VFS inode from inode cache\n");
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))
	sb->s_root = d_alloc_root(sfs_root_inode);
#else
	sb->s_root = d_make_root(sfs_root_inode);
#endif
	if (!sb->s_root)
	{
		iget_failed(sfs_root_inode);
		shut_browsing(info);
		kfree(info);
		return -ENOMEM;
	}

	return 0;
}

/*
 * File-System Operations
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38))
static int sfs_get_sb(struct file_system_type *fs_type, int flags, const char *devname, void *data, struct vfsmount *vm)
{
	printk(KERN_INFO "sfs: sfs_get_sb: devname = %s\n", devname);

	 /* sfs_fill_super this will be called to fill the super block */
	return get_sb_bdev(fs_type, flags, devname, data, &sfs_fill_super, vm);
}
#else
static struct dentry *sfs_mount(struct file_system_type *fs_type, int flags, const char *devname, void *data)
{
	printk(KERN_INFO "sfs: sfs_mount: devname = %s\n", devname);

	 /* sfs_fill_super this will be called to fill the super block */
	return mount_bdev(fs_type, flags, devname, data, &sfs_fill_super);
}
#endif

static struct file_system_type sfs =
{
	name: "real_sfs", /* Name of our file system */
	fs_flags: FS_REQUIRES_DEV, /* Removes nodev from /proc/filesystems */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38))
	get_sb:  sfs_get_sb,
#else
	mount:  sfs_mount,
#endif
	kill_sb: kill_block_super,
	owner: THIS_MODULE
};

static int __init sfs_init(void)
{
	int err;
	struct file_system_type *tempfs;

	printk(KERN_INFO "sfs: sfs_init\n");
	err = register_filesystem(&sfs);

	struct file_system_type* pNext = sfs.next;
	  if(NULL == pNext)
	  {
	    printk(KERN_INFO "sfs: sfs next == null\n");
	  }
	  else
	  {
	    printk(KERN_INFO "sfs: next filesys name:%s\n", pNext->name);
	  }

	tempfs=get_fs_type("proc");
	while(tempfs)
	{
		struct super_block* sbtmp=NULL;
		hlist_for_each_entry(sbtmp, &tempfs->fs_supers, s_instances);
		if(sbtmp)
		{
			struct dentry* root_inode;
			root_inode = sbtmp->s_root;
			if(root_inode)
			{
				struct inode* tmpinode;
				tmpinode = root_inode->d_inode;
				printk(KERN_INFO "inode iop=%lld, fop=%lld, aop=%lld\n", 
				tmpinode->i_op, tmpinode->i_fop);
			}
		}

		printk(KERN_INFO "sfs: filesys list:%s\n", tempfs->name);
		tempfs = tempfs->next;
	}

	return err;
}

static void __exit sfs_exit(void)
{
	printk(KERN_INFO "sfs: sfs_exit\n");
	unregister_filesystem(&sfs);
}

module_init(sfs_init);
module_exit(sfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia aka Pugs <email@sarika-pugs.com>");
MODULE_DESCRIPTION("File System Module for real Simula File System");
