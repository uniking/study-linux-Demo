#ifndef REAL_SFS_DS_H
#define REAL_SFS_DS_H

#ifdef __KERNEL__
#include <linux/fs.h>
#include <linux/spinlock.h>
#endif

#define SIMULA_FS_TYPE 0x13090D15 /* Magic Number for our file system */
#define SIMULA_FS_BLOCK_SIZE 512 /* in bytes */
#define SIMULA_FS_BLOCK_SIZE_BITS 9 /* log(SIMULA_FS_BLOCK_SIZE) w/ base 2 */
#define SIMULA_FS_ENTRY_SIZE 64 /* in bytes */
#define SIMULA_FS_FILENAME_LEN 15
#define SIMULA_FS_DATA_BLOCK_CNT ((SIMULA_FS_ENTRY_SIZE - ((SIMULA_FS_FILENAME_LEN + 1) + 3 * 4)) / 4)

typedef unsigned char byte1_t;
typedef unsigned int byte4_t;
typedef unsigned long long byte8_t;

typedef struct sfs_super_block
{
	byte4_t type; /* Magic number to identify the file system */
	byte4_t block_size; /* Unit of allocation */
	byte4_t partition_size; /* in blocks */
	byte4_t entry_size; /* in bytes */
	byte4_t entry_table_size; /* in blocks */
	byte4_t entry_table_block_start; /* in blocks */
	byte4_t entry_count; /* Total entries in the file system */
	byte4_t data_block_start; /* in blocks */
	byte4_t reserved[SIMULA_FS_BLOCK_SIZE / 4 - 8];
} sfs_super_block_t; /* Making it of SIMULA_FS_BLOCK_SIZE */

typedef struct sfs_file_entry
{
	char name[SIMULA_FS_FILENAME_LEN + 1];
	byte4_t size; /* in bytes */
	byte4_t timestamp; /* Seconds since Epoch */
	byte4_t perms; /* Permissions only for user; Replicated for group & others */
	byte4_t blocks[SIMULA_FS_DATA_BLOCK_CNT];
} sfs_file_entry_t;

#ifdef __KERNEL__
typedef struct sfs_info
{
	struct super_block *vfs_sb; /* Super block structure from VFS for this fs */
	sfs_super_block_t sb; /* Our fs super block */
	byte1_t *used_blocks; /* Used blocks tracker */
	spinlock_t lock; /* Used for protecting used_blocks access */
} sfs_info_t;
#endif

#endif
