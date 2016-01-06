#include "common.h"

#define long_size 4

int WriteProcessMemory(int pid, char *BaseAddr, char* Buffer, unsigned long Size)
{
	int iRtn = 0;
	int i=0;
	int j = Size/long_size;
	char* laddr = Buffer;

	union u{
		long val;
		char chars[long_size];
	}data;

	while(i < j)
	{
		memcpy(data.chars, laddr, long_size);

		iRtn = ptrace(PTRACE_POKEDATA, pid, BaseAddr+i*long_size, data.val);
		if(-1 == iRtn)
		{
			break;
		}
		
		i++;
		laddr += long_size;
	}

	j = Size % long_size;
	if(j != 0)
	{
		data.val = ptrace(PTRACE_PEEKDATA, pid, BaseAddr+i*long_size, NULL);
		memcpy(data.chars, laddr, j);

		iRtn = ptrace(PTRACE_POKEDATA, pid, BaseAddr+i*long_size, data.val);
	}
	
	return iRtn;
}
