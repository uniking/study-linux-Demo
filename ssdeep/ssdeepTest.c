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
    timespec clock1, clock2;
    time_t timer1, timer2;
    struct tm* tblock1;
    struct tm* tblock2;

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
        timer1 = time(NULL);
        tblock1 = localtime(&timer1);
        printf("%s\n", asctime(tblock1)); 
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock1);
        while(count>0)
        {
            s = fuzzy_compare(result1, result2);
            count--;
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock2);
        timer2 = time(NULL);
        tblock2 = localtime(&timer2);
        printf("%s\n", asctime(tblock2));       

        printf("start=%lld end=%lld clock=%lld\n", clock1.tv_nsec, clock2.tv_nsec, clock2.tv_nsec-clock1.tv_nsec);

	    printf("相似度:%d\n", s);
    }
    else
    {
        printf("%s file\n", argv[0]);
        printf("%s src des\n", argv[0]);
    }


	return 0;
}
