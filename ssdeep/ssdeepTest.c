#include <fuzzy.h>
#include <time.h>

/*
指纹  文件路径 秘级 其他
*/

int main(int argn, char* argv[])
{
	char result1[1024] = {0};
	char result2[1024] = {0};
   int count = 500000;
   int s = 0;
    timespec time1, time2;

    if(argn == 2)
    {
        fuzzy_hash_filename(argv[1], result1);
        printf("fhash:%s\n", result1);
    }
    else if(argn == 3)
    {
	    if(fuzzy_hash_filename(argv[1], result1))
	    {
		    printf("fuzzy_hash_filename error\n");
	    }

	    if(fuzzy_hash_filename(argv[2], result2))
	    {
		    printf("fuzzy_hash_filename error\n");
	    }
	    printf("hash指纹:%s\n", result1);
	    printf("hash指纹:%s\n",result2);


        printf("compare times=%d\n", count);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
        while(count>0)
        {
            s = fuzzy_compare(result1, result2);
            count--;
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
        printf("time=%d\n", time2.tv_nsec-time1.tv_nsec);

	    printf("相似度:%d\n", s);
    }
    else
    {
        printf("%s file\n", argv[0]);
        printf("%s src des\n", argv[0]);
    }


	return 0;
}
