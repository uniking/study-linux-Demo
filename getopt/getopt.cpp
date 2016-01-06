#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	/*
	./getopt -aWang -bZhi -cLong
	第三个参数为参数列表 -a -b -c -hI -i -l
	*/
	int opt = getopt(argc, argv, "a:b:c:hI:i:l:M:m:n:p:q:t:");
	switch(opt)
	{
		case 'a':
			printf("a:%s\n", optarg);
			break;
		case 'b':
			printf("b:%s\n", optarg);
			break;
		case 'c':
			printf("c:%s\n", optarg);
			break;
		default:
			printf("default:%x", opt);
	}
}
