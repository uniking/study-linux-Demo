#include <stdio.h>


extern "C"
{
	#include "testc.h"
}

int main(int argn, const char* argv[])
{
	if(!winDllInit())
		printf("winDllInit error\n");

	winMain_7z(argn, argv);
	return 0;
}
