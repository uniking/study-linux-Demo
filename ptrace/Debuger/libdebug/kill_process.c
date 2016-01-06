#include "common.h"

int KillProcess(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_KILL, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace kill error\n");

	return iRtn;
}
