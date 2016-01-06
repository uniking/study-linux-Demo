#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

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

static void *
sig_thread(void *arg)
{
	sigset_t *set = (sigset_t*)arg;
	int s, sig;

	for (;;) 
	{
		s = sigwait(set, &sig);
		if (s != 0)
			printf("sigwait error\n");
		sig_usr(sig);
	}
}


main()
{
	sigset_t set;
	pthread_t sigthread;

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	int s;

	s = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(s != 0)
	{
		printf("pthread_sigmask error\n");
	}
	else
	{
		//该线程继承了现在进程的sigset
		s = pthread_create(&sigthread, NULL, sig_thread, (void*) &set);
		if(s != 0)
		{
			printf("pthread_create sig_thread error\n");
		}
	}

	for(;;)
		pause();
}
