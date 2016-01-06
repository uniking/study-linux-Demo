#include <stdio.h>
extern "C"
{
int myadd(int , int);
}

int main()
{
	printf("hello mingw %d\n", myadd(12, 12));
	return 0;
}
