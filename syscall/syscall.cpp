#include <unistd.h>
#include <sys/syscall.h>

#include <iostream>
using namespace std;

int main()
{
	int thr = syscall(SYS_gettid);
	cout<<"thread id="<<thr<<endl;
	return 0;
}
