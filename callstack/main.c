#include <execinfo.h>
#include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>

#define BT_BUF_SIZE 100

void
       myfunc3(void)
       {
           int j, nptrs;
           void *buffer[BT_BUF_SIZE];
           char **strings;

           nptrs = backtrace(buffer, BT_BUF_SIZE);
           printf("backtrace() returned %d addresses\n", nptrs);

           /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
              would produce similar output to the following: */

           strings = backtrace_symbols(buffer, nptrs);
           if (strings == NULL) {
               perror("backtrace_symbols");
               exit(EXIT_FAILURE);
           }

           for (j = 0; j < nptrs; j++)
               printf("%s\n", strings[j]);

           free(strings);
       }


int main()
{
	myfunc3();
	return 0;
}
