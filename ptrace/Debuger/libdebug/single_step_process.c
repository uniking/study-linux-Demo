#include "common.h"

int SingleStepProcess(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace single step error\n");

	return 0;
}
