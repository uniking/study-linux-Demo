//fork wait waitpid signal pause kill raise exec exit

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

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

int main()
{
	pid_t pid = fork();
	if(pid == 0)
	{//child
		say_hello("child", 5);
		execl("/use/bin/ls", "ls", "/home/sxis", NULL);
	}
	else
	{//parent
		int stat_loc;
		pid_t wip = wait(&stat_loc);
		cout<<"wip:"<<wip<<" stat_loc:"<<stat_loc<<endl;
		cout<<"child pid:"<<pid<<endl;
	}
}
