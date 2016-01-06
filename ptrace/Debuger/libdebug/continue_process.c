#include "common.h"

int ContinueProcess(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_CONT, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace continue error\n");

	return iRtn;
}
