#include <common.h>

int attach(int pid)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	if(-1 == iRtn)
		printf("ptrace error\n");

	return iRtn;
}

int OpenProcess(int pid)
{
	return attach(pid);
}
