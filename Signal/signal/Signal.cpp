#include <signal.h>
#include <stdio.h>
#include <unistd.h>

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

main()
{
	if(signal(SIGUSR1, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if(signal(SIGUSR2, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR2\n");
	if(signal(SIGTERM, sig_usr) == SIG_ERR)
		printf("can't catch SIGTERM\n");//本sig默认是终止

	for(;;)
		pause();
}
