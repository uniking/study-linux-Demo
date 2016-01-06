#include "common.h"

int CloseProcess(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_DETACH, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace detach error\n");

	return ;
}
