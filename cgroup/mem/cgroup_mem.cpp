#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

void init_cgroup()
{
/*
# 创建memory cgroup
$ mkdir /sys/fs/cgroup/memory/haoel
$ echo 64k > /sys/fs/cgroup/memory/haoel/memory.limit_in_bytes
*/

	mkdir("/sys/fs/cgroup/memory/haoel", 755);
	system("echo 32k > /sys/fs/cgroup/memory/haoel/memory.limit_in_bytes");

/*
# 把上面的进程的pid加入这个cgroup
$ echo [pid] > /sys/fs/cgroup/memory/haoel/tasks
*/
	char buf[256];
	snprintf(buf, 256, "echo %ld >> /sys/fs/cgroup/memory/haoel/tasks", syscall(SYS_gettid));
	system(buf);
}
 
int main(void)
{
	int size = 0;
	int chunk_size = 1024;
	void *p = NULL;
	
	init_cgroup();

	while(1)
	{
		if ((p = (void*)malloc(chunk_size)) == NULL)
		{
			printf("out of memory!!\n");
			break;
		}
		memset(p, 1, chunk_size);
		size += chunk_size;
		printf("[%d] - memory is allocated [%8d] bytes \n", getpid(), size);
		sleep(1);
	}
	return 0;
}
