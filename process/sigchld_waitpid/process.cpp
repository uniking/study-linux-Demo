//fork wait waitpid signal pause kill raise exec exit

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;

void say_hello(char* mark, int times)
{
	int i=0;
	while(i < times)
	{
		sleep(1);
		cout<<mark<<" "<<i<<endl;
		i++;
	}

	exit(1);
}

void make_chld(int keep)
{
	pid_t pid = fork();
	if(pid == 0)
	{//child
		say_hello("child", keep);
		execl("/use/bin/ls", "/home/sxis");
		exit(0);
	}
}

int main()
{
	make_chld(6);
	make_chld(6);
	make_chld(6);
	make_chld(6);
	make_chld(60);
	sleep(1);

	int pid;
	int status;
	//unblocked
	while ( (pid = waitpid(-1, &status, WNOHANG)) > 0 )
		printf("child %d terminated\n", pid);

	printf("exit main\n");
	sleep(65);
	while ( (pid = waitpid(-1, &status, WNOHANG)) > 0 )
		printf("child %d terminated\n", pid);
	printf("exit main\n");

	// -1     meaning wait for any child process.
	// WNOHANG     return immediately if no child has exited.

	sleep(10);
}
