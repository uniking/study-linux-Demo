#include <syslog.h>
#include <stdio.h>
int main(int argn, char* argv[])
{
    int i=0;
    openlog("testLog", LOG_CONS | LOG_PID, 0);
    syslog(LOG_DEBUG, "This is a syslog test message\n");
    printf("syslog return %d\n", i);
    closelog();

    return 0;
}
