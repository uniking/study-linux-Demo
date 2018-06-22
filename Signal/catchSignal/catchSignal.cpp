#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <string>
using namespace std;

int main()
{
	char name[1] = {0};
	open(name, O_RDONLY);
	creat(name, O_RDONLY);


	char cmd[1024];
	pid_t pid = getpid();
	sprintf(cmd, "/proc/%d/stack", pid);
	
	FILE* file_process = popen(cmd, "r");	
	fgets(cmd, 1024, file_process);


	unsigned char* point_byte = (unsigned char*)open;
	for(int i=0; i<20; ++i)
		printf("%02x ",point_byte[i]);
	printf("\n");

	printf("%s\n", cmd);
	printf("exit ok\n");

	return 0;
}
