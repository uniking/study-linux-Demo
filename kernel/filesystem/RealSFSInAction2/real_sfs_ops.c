#include <linux/version.h> /* For LINUX_VERSION_CODE */
#include <linux/fs.h> /* For struct super_block */
#include <linux/errno.h> /* For error codes */
#include <linux/buffer_head.h> /* struct buffer_head, sb_bread, ... */
#include <linux/string.h> /* For memcpy */
#include <linux/vmalloc.h> /* For vmalloc, ... */
#include <linux/time.h> /* For get_seconds, ... */

#include "real_sfs_ds.h"
#include "real_sfs_ops.h"

static int read_sb_from_real_sfs(sfs_info_t *info, sfs_super_block_t *sb)
{
	struct buffer_head *bh;

	if (!(bh = sb_bread(info->vfs_sb, 0 /* Super block is the 0th block */)))
	{
		return -EIO;
	}
	memcpy(sb, bh->b_data, SIMULA_FS_BLOCK_SIZE);
	brelse(bh);
	return 0;
}
static int read_from_real_sfs(sfs_info_t *info, byte4_t block, byte4_t offset, void *buf, byte4_t len)
{
	byte4_t block_size = info->sb.block_size;
	byte4_t bd_block_size = info->vfs_sb->s_bdev->bd_block_size;
	byte4_t abs;
	struct buffer_head *bh;

	// Translating the real SFS block numbering to underlying block device block numbering, for sb_bread()
	abs = block * block_size + offset;
	block = abs / bd_block_size;
	offset = abs % bd_block_size;
	if (offset + len > bd_block_size) // Should never happen
	{
		return -EINVAL;
	}
	if (!(bh = sb_bread(info->vfs_sb, block)))
	{
		return -EIO;
	}
	memcpy(buf, bh->b_data + offset, len);
	brelse(bh);
	return 0;
}
static int write_to_real_sfs(sfs_info_t *info, byte4_t block, byte4_t offset, void *buf, byte4_t len)
{
	byte4_t block_size = info->sb.block_size;
	byte4_t bd_block_size = info->vfs_sb->s_bdev->bd_block_size;
	byte4_t abs;
	struct buffer_head *bh;

	// Translating the real SFS block numbering to underlying block device block numbering, for sb_bread()
	abs = block * block_size + offset;
	block = abs / bd_block_size;
	offset = abs % bd_block_size;
	if (offset + len > bd_block_size) // Should never happen
	{
		return -EINVAL;
	}
	if (!(bh = sb_bread(info->vfs_sb, block)))
	{
		return -EIO;
	}
	memcpy(bh->b_data + offset, buf, len);
	mark_buffer_dirty(bh);
	brelse(bh);
	return 0;
}
static int read_entry_from_real_sfs(sfs_info_t *info, int ino, sfs_file_entry_t *fe)
{
	return read_from_real_sfs(info, info->sb.entry_table_block_start,
				ino * sizeof(sfs_file_entry_t), fe, sizeof(sfs_file_entry_t));
}
static int write_entry_to_real_sfs(sfs_info_t *info, int ino, sfs_file_entry_t *fe)
{
	return write_to_real_sfs(info, info->sb.entry_table_block_start,
				ino * sizeof(sfs_file_entry_t), fe, sizeof(sfs_file_entry_t));
}

int init_browsing(sfs_info_t *info)
{
	byte1_t *used_blocks;
	int i, j;
	sfs_file_entry_t fe;
	int retval;

	if ((retval = read_sb_from_real_sfs(info, &info->sb)) < 0)
	{
		return retval;
	}
	if (info->sb.type != SIMULA_FS_TYPE)
	{
		printk(KERN_ERR "Invalid SFS detected. Giving up.\n");
		return -EINVAL;
	}

	/* Mark used blocks */
	used_blocks = (byte1_t *)(vmalloc(info->sb.partition_size));
	if (!used_blocks)
	{
		return -ENOMEM;
	}
	for (i = 0; i < info->sb.data_block_start; i++)
	{
		used_blocks[i] = 1;
	}
	for (; i < info->sb.partition_size; i++)
	{
		used_blocks[i] = 0;
	}

	for (i = 0; i < info->sb.entry_count; i++)
	{
		if ((retval = read_entry_from_real_sfs(info, i, &fe)) < 0)
		{
			vfree(used_blocks);
			return retval;
		}
		if (!fe.name[0]) continue;
		for (j = 0; j < SIMULA_FS_DATA_BLOCK_CNT; j++)
		{
			if (fe.blocks[j] == 0) break;
			used_blocks[fe.blocks[j]] = 1;
		}
	}

	info->used_blocks = used_blocks;
	info->vfs_sb->s_fs_info = info;
	spin_lock_init(&info->lock);
	return 0;
}
void shut_browsing(sfs_info_t *info)
{
	if (info->used_blocks)
		vfree(info->used_blocks);
}

int sfs_get_data_block(sfs_info_t *info)
{
	int i;

	spin_lock(&info->lock); // To prevent racing on used_blocks access
	for (i = info->sb.data_block_start; i < info->sb.partition_size; i++)
	{
		if (info->used_blocks[i] == 0)
		{
			info->used_blocks[i] = 1;
			spin_unlock(&info->lock);
			return i;
		}
	}
	spin_unlock(&info->lock);
	return INV_BLOCK;
}
void sfs_put_data_block(sfs_info_t *info, int i)
{
	spin_lock(&info->lock); // To prevent racing on used_blocks access
	info->used_blocks[i] = 0;
	spin_unlock(&info->lock);
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
int sfs_list(sfs_info_t *info, struct file *file, void *dirent, filldir_t filldir)
{
	loff_t pos;
	int ino;
	sfs_file_entry_t fe;
	int retval;

	pos = 1; /* Starts at 1 as . is position 0 & .. is position 1 */
	for (ino = 0; ino < info->sb.entry_count; ino++)
	{
		if ((retval = read_entry_from_real_sfs(info, ino, &fe)) < 0)
			return retval;
		if (!fe.name[0]) continue;
		pos++; /* Position of this file */
		if (file->f_pos == pos)
		{
			printk(KERN_INFO "sfs: read dir data seg:%s\n", fe.name);
			retval = filldir(dirent, fe.name, strlen(fe.name), file->f_pos, S2V_INODE_NUM(ino), DT_REG);
			if (retval)
			{
				return retval;
			}
			file->f_pos++;
		}
	}
	return 0;
}
#else
int sfs_list(sfs_info_t *info, struct file *file, struct dir_context *ctx)
{
	loff_t pos;
	int ino;
	sfs_file_entry_t fe;
	int retval;

	pos = 1; /* Starts at 1 as . is position 0 & .. is position 1 */
	for (ino = 0; ino < info->sb.entry_count; ino++)
	{
		if ((retval = read_entry_from_real_sfs(info, ino, &fe)) < 0)
			return retval;
		if (!fe.name[0]) continue;
		pos++; /* Position of this file */
		if (ctx->pos == pos)
		{
			if (!dir_emit(ctx, fe.name, strlen(fe.name), S2V_INODE_NUM(ino), DT_REG))
			{
				return -ENOSPC;
			}
			ctx->pos++;
		}
	}
	return 0;
}
#endif
int sfs_create(sfs_info_t *info, char *fn, int perms, sfs_file_entry_t *fe)
/* This function is called only if the file doesn't exist */
{
	int ino, free_ino, i;

	free_ino = INV_INODE;
	for (ino = 0; ino < info->sb.entry_count; ino++)
	{
		if (read_entry_from_real_sfs(info, ino, fe) < 0)
			return INV_INODE;
		if (!fe->name[0])
		{
			free_ino = ino;
			break;
		}
	}
	if (free_ino == INV_INODE)
	{
		printk(KERN_ERR "No entries left\n");
		return INV_INODE;
	}

	strncpy(fe->name, fn, SIMULA_FS_FILENAME_LEN);
	fe->name[SIMULA_FS_FILENAME_LEN] = 0;
	fe->size = 0;
	fe->timestamp = get_seconds();
	fe->perms = perms;
	for (i = 0; i < SIMULA_FS_DATA_BLOCK_CNT; i++)
	{
		fe->blocks[i] = 0;
	}

	if (write_entry_to_real_sfs(info, free_ino, fe) < 0)
		return INV_INODE;

	return S2V_INODE_NUM(free_ino);
}
int sfs_lookup(sfs_info_t *info, char *fn, sfs_file_entry_t *fe)
{
	int ino;

	for (ino = 0; ino < info->sb.entry_count; ino++)
	{
		if (read_entry_from_real_sfs(info, ino, fe) < 0)
			return INV_INODE;
		if (!fe->name[0]) continue;
		if (strcmp(fe->name, fn) == 0) return S2V_INODE_NUM(ino);
	}

	return INV_INODE;
}
int sfs_remove(sfs_info_t *info, char *fn)
{
	int vfs_ino, block_i;
	sfs_file_entry_t fe;

	if ((vfs_ino = sfs_lookup(info, fn, &fe)) == INV_INODE)
	{
		printk(KERN_ERR "File %s doesn't exist\n", fn);
		return INV_INODE;
	}
	/* Free up all allocated blocks, if any */
	for (block_i = 0; block_i < SIMULA_FS_DATA_BLOCK_CNT; block_i++)
	{
		if (!fe.blocks[block_i])
		{
			break;
		}
		sfs_put_data_block(info, fe.blocks[block_i]);
	}
	memset(&fe, 0, sizeof(sfs_file_entry_t));

	if (write_entry_to_real_sfs(info, V2S_INODE_NUM(vfs_ino), &fe) < 0)
		return INV_INODE;

	return vfs_ino;
}
int sfs_get_file_entry(sfs_info_t *info, int vfs_ino, sfs_file_entry_t *fe)
{
	return read_entry_from_real_sfs(info, V2S_INODE_NUM(vfs_ino), fe);
}
int sfs_update(sfs_info_t *info, int vfs_ino, int *size, int *timestamp, int *perms)
{
	sfs_file_entry_t fe;
	int i;
	int retval;

	if ((retval = sfs_get_file_entry(info, vfs_ino, &fe)) < 0)
	{
		return retval;
	}
	if (size) fe.size = *size;
	if (timestamp) fe.timestamp = *timestamp;
	if (perms && (*perms <= 07)) fe.perms = *perms;

	for (i = (fe.size + info->sb.block_size - 1) / info->sb.block_size; i < SIMULA_FS_DATA_BLOCK_CNT; i++)
	{
		if (fe.blocks[i])
		{
			sfs_put_data_block(info, fe.blocks[i]);
			fe.blocks[i] = 0;
		}
	}

	return write_entry_to_real_sfs(info, V2S_INODE_NUM(vfs_ino), &fe);
}
int sfs_update_file_entry(sfs_info_t *info, int vfs_ino, sfs_file_entry_t *fe)
{
	return write_entry_to_real_sfs(info, V2S_INODE_NUM(vfs_ino), fe);
}
