#include <time.h>
#include <stdio.h>

int main(int argn, char* argv[])
{
    timespec time1, time2;
    time_t timer1, timer2;
    struct tm* t;

    /////////////////////////////////////
    //get cpu time from process start to this code
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    
    //////////////////////////////////////
    // local time
    timer1 = time(NULL);
    t = localtime(&timer1);
    printf("%d %d %d %d %d %d\n",t->tm_yday, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    printf("%s\n", asctime(t));
    //printf("%d %d %s\n", timer2, time2);



	return 0;
}
