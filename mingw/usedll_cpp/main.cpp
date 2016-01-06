#include <stdio.h>
extern "C"
{
	int myadd(int , int);
}
int main()
{
	int a = myadd(10, 20);
	printf("a=%d\n", a);
	return 0;
}
