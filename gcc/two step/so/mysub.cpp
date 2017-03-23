#include <stdio.h>

int my_sub(int a, int b)
{
	return a-b;
}


__attribute ((constructor)) void sub_init(void)
{
	printf("mysub init\n");
}

__attribute ((destructor)) void sub_finit(void)
{
	printf("mysub finit\n");
}

