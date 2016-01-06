#include <string.h>
#include <stdio.h>

// ./strtok wang,zhi,long
// 不能是./strtok wang|zhi|long， |是linux的管道
int main(int argc, char* argv[])
{
	char arg[256]={0};
	char* saveptr;
	char* str;
	
	if(argc != 2)
	{
		printf("%s wang,zhi,long\n", argv[0]);
		return 0;
	}

	strcpy(arg, argv[1]);
	arg[strlen(argv[1])] = 0;

	str = strtok_r(arg, ",", &saveptr);
	while(str)
	{
		printf("%s\n", str);
		str = strtok_r(NULL, ",", &saveptr);
	}
	return 0;
}
