#include <stdio.h>
#include <dlfcn.h>

int main()
{
	printf("hello mingw\n");
	void *libmy;
	libmy = dlopen("./testdll.dll", RTLD_NOW | RTLD_GLOBAL);
	if(libmy == 0)
		printf("dlopen error\n");
	int (*f2)(char *);
	f2 = (int (*)(char *)) dlsym(libmy, "myadd");
	return 0;
}
