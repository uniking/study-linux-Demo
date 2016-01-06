#include <stdio.h>

void dump(int num)
{
	int *a = NULL;

	*a = 12;

	return;
}

int main()
{
	dump(10);

	return 0;
}
