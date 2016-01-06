#include <syslog.h>
#include <stdio.h>
int main()
{
    int i=0;
    openlog("testLog", LOG_CONS | LOG_PID, 0);
    i = syslog(LOG_DEBUG, "This is a syslog test message\n", 10);
    printf("syslog return %d\n", i);
    closelog();

    return 0;
}
