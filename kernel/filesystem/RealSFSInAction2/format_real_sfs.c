#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h> /* For errno */
#include <string.h> /* For strerror() */
#include <sys/ioctl.h> /* For ioctl() */
#include <linux/fs.h> /* For BLKGETSIZE64 */

#include "real_sfs_ds.h"

#define SFS_ENTRY_RATIO 0.10 /* 10% of all blocks */
#define SFS_ENTRY_TABLE_BLOCK_START 1

sfs_super_block_t sb =
{
	.type = SIMULA_FS_TYPE,
	.block_size = SIMULA_FS_BLOCK_SIZE,
	.entry_size = SIMULA_FS_ENTRY_SIZE,
	.entry_table_block_start = SFS_ENTRY_TABLE_BLOCK_START
};
sfs_file_entry_t fe; /* All 0's */

void write_super_block(int sfs_handle, sfs_super_block_t *sb)
{
	write(sfs_handle, sb, sizeof(sfs_super_block_t));
}
void clear_file_entries(int sfs_handle, sfs_super_block_t *sb)
{
	int i;
	byte1_t block[SIMULA_FS_BLOCK_SIZE];

	for (i = 0; i < sb->block_size / sb->entry_size; i++)
	{
		memcpy(block + i * sb->entry_size, &fe, sizeof(fe));
	}
	for (i = 0; i < sb->entry_table_size; i++)
	{
		write(sfs_handle, block, sizeof(block));
	}
}

int main(int argc, char *argv[])
{
	int sfs_handle;
	byte8_t size;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <partition's device file>\n", argv[0]);
		return 1;
	}
	sfs_handle = open(argv[1], O_RDWR);
	if (sfs_handle == -1)
	{
		fprintf(stderr, "Error formatting %s: %s\n", argv[1], strerror(errno));
		return 2;
	}
	if (ioctl(sfs_handle, BLKGETSIZE64, &size) == -1)
	{
		fprintf(stderr, "Error getting size of %s: %s\n", argv[1], strerror(errno));
		return 3;
	}
	sb.partition_size = size / SIMULA_FS_BLOCK_SIZE;
	sb.entry_table_size = sb.partition_size * SFS_ENTRY_RATIO;
	sb.entry_count = sb.entry_table_size * sb.block_size / sb.entry_size;
	sb.data_block_start = SFS_ENTRY_TABLE_BLOCK_START +  sb.entry_table_size;

	printf("Partitioning %Ld byte sized %s ... ", size, argv[1]);
	fflush(stdout);
	write_super_block(sfs_handle, &sb);
	clear_file_entries(sfs_handle, &sb);

	close(sfs_handle);
	printf("done\n");

	return 0;
}
