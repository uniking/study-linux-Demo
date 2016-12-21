#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int create_mfd(char* buffer, size_t length, int& fd)
{
	FILE* filed = fmemopen(buffer, length, "r+");
	if(filed == NULL)
	{
		printf("create_mfd fmemopen error, %s\n", strerror(errno));
		return -1;
	}

	//void* p = mmap(buffer, length, PROT_READ, MAP_SHARED, fd, 0);
	fd = fileno(filed);
	if(fd == -1)
	{
		printf("create_mfd fileno error, %s\n", strerror(errno));
	}

	{
		char buf[64]={0};
		fread(buf, length, length, filed);
		printf("fread %s\n", buf);
	}

	return fd;
}

int close_mfd(char* buffer, size_t length, int fd)
{
	//munmap(buffer, length);
	//close(fd);
}

int main()
{
	char buf[512];
	snprintf(buf, 512, "hello world");

	int fd;
	create_mfd(buf, 11, fd);

	char rbuf[32]={0};
	int rn = read(fd, rbuf, 5);
	if(rn == -1)
		printf("main read error %s\n", strerror(errno));
	else
		printf("%d %s", rn, rbuf);

	close_mfd(buf, 11, fd);

	return 0;
}

