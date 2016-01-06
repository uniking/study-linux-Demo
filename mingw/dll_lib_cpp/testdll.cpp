#include <stdio.h>

#define EXPORT_FUN __declspec(dllexport)

int EXPORT_FUN myadd(int a, int b)
{
	printf("hello mingw\n");
	return a+b;
}
