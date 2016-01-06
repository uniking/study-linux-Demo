#include <fuzzy.h>

int main()
{
	char result1[1024] = {0};
	char result2[1024] = {0};

	if(fuzzy_hash_filename("./1.bmp", result1))
	{
		printf("fuzzy_hash_filename error\n");
	}

	if(fuzzy_hash_filename("./2.bmp", result2))
	{
		printf("fuzzy_hash_filename error\n");
	}
	printf("hash指纹:%s\n", result1);
	printf("hash指纹:%s\n",result2);

	int s = fuzzy_compare(result1, result2);
	printf("相似度:%d\n", s);

	return 0;
}
