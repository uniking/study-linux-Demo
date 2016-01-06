#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void
sig_usr(int signo)
{
	if(signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if(signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else if(signo == SIGTERM)
		printf("received SIGTERM\n");
	else
		printf("received signel %d\n", signo);
}

int main()
{
	//int3

	asm(
	"int3"
	);
	if(signal(SIGUSR1, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR1\n");

	for(;;)
	{
		sleep(1);
	}
}
