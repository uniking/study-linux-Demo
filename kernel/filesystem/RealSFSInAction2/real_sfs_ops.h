#ifndef REAL_SFS_OPS_H
#define REAL_SFS_OPS_H

#include <linux/version.h>
#include <linux/fs.h>

#include "real_sfs_ds.h"

/*
 * Inode number 0 is not treated as a valid inode number at many places,
 * including applications like ls. So, the inode numbering should start
 * at not less than 1. For example, making root inode's number 0, leads
 * the lookuped entries . & .. to be not shown in ls
 */
#define ROOT_INODE_NUM (1)
#define S2V_INODE_NUM(i) (ROOT_INODE_NUM + 1 + (i)) // SFS to VFS
#define V2S_INODE_NUM(i) ((i) - (ROOT_INODE_NUM + 1)) // VFS to SFS
#define INV_INODE (-1)
#define INV_BLOCK (-1)

int init_browsing(sfs_info_t *info);
void shut_browsing(sfs_info_t *info);

int sfs_get_data_block(sfs_info_t *info); // Returns block number or INV_BLOCK
void sfs_put_data_block(sfs_info_t *info, int i);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
int sfs_list(sfs_info_t *info, struct file *file, void *dirent, filldir_t filldir);
#else
int sfs_list(sfs_info_t *info, struct file *file, struct dir_context *ctx);
#endif

/* The following 3 APIs returns VFS inode number or INV_INODE */
int sfs_create(sfs_info_t *info, char *fn, int perms, sfs_file_entry_t *fe);
int sfs_lookup(sfs_info_t *info, char *fn, sfs_file_entry_t *fe);
int sfs_remove(sfs_info_t *info, char *fn);

int sfs_get_file_entry(sfs_info_t *info, int vfs_ino, sfs_file_entry_t *fe);
int sfs_update(sfs_info_t *info, int vfs_ino, int *size, int *timestamp, int *perms);
int sfs_update_file_entry(sfs_info_t *info, int vfs_ino, sfs_file_entry_t *fe);

#endif
