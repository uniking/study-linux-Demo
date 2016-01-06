#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int filedes;
	int ex;
	
	//防止多进程
	if((ex = open("/tmp/myExcl", O_RDWR | O_CREAT | O_EXCL, 0666) ) == -1)
	{
		printf("have one running\n");
		return -1;
	}

	write(STDOUT_FILENO, "Hello stdout\n", strlen("Hello stdout\n"));

	filedes = open("./test.txt", O_RDWR | O_CREAT, 0666);
	printf("filedes=%d\n", filedes);
	write(filedes, "Hello stdout\n", strlen("Hello stdout\n"));
	close(filedes);

	sleep(20);
	close(ex);
	remove("/tmp/myExcl");
	return -1;
}
