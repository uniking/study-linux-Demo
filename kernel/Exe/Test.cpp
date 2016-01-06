#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	char buffer[1024]={0};
	if(argc == 1)
	{
		printf("Test read devicename content\n");
		printf("Test write devicename content\n");
	}
	else if(argc < 4)
	{
		printf("para fail\n");
	}

	char* oper = argv[1];
	char* devname = argv[2];
	char* content = argv[3];
	int filedes=-1;

	if(strncmp(oper, "read", 4) == 0)
	{
		filedes = open(argv[2], O_RDONLY);
		if(filedes == -1)
		{
			printf("open error\n");
			return 0;
		}

		read(filedes, buffer, 1024);
		printf(buffer);
	}
	else if(strncmp(oper, "write", 4) == 0)
	{
		filedes = open(argv[2], O_RDWR);
		if(filedes == -1)
		{
			printf("open error\n");
			return 0;
		}
		write(filedes, content, strlen(content));
	}
	else
	{
		printf("unknow operation\n");
	}

	if(filedes != -1)
	{
		close(filedes);
	}

	
	return 0;
}
