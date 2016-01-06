#include <magic.h>
#include <stdio.h>

void test()
{
	magic_t ms = magic_open(MAGIC_NONE);
	if(ms == NULL)
	{
		printf("magic_open error\n");
		return;
	}

	if(magic_load(ms, NULL) == -1)
	{
		printf("magic load error\n");
		return;
	}

	const char* type = magic_file(ms, "filemap");
	printf("%s\n", type);
}

int main()
{
	test();
	return 0;
}
