#include <stdio.h>

int my_sbb(int a, int b)
{
	return a-b;
}

__attribute ((constructor)) void sbb_init(void)
{
	printf("mysbb init\n");
}

__attribute ((destructor)) void sbb_finit(void)
{
	printf("mysbb finit\n");
}
