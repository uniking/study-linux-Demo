#include <stdio.h>

extern int my_sub(int a, int b);

int main()
{
	my_sub(12, 34);
	my_sub(129, 34);
	return 0;
}

__attribute ((constructor)) void main_init(void)
{
	printf("main init\n");
}

__attribute ((destructor)) void main_finit(void)
{
	printf("main finit\n");
}

