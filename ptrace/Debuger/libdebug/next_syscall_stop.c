#include "common.h"

int NextSyscallStopProcess(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace next syscall stop error\n");

	return 0;
}
