#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/shm.h>

//share memory

int main()
{
	int pid;
	pid = fork();
	if(pid < 0)
	{
		printf("fork error\n");
		return 1;
	}
	else if(pid == 0)
	{//父进程创建共享内存
		int shmid;
		void* pshm = NULL;
		shmid = shmget((key_t)12345, 1024, 0666|IPC_CREAT);
		if(shmid == -1)
			return 1;
		pshm = shmat(shmid, 0, 0);
		strcpy((char*)pshm, "hellow world");

		shmdt(pshm);
		
	}
	else
	{
		int shmid;
		void* pshm = NULL;

		sleep(2);
		shmid = shmget((key_t)12345, 1024, 0666|IPC_CREAT);
		if(shmid == -1)
			return 1;
		pshm = shmat(shmid, 0, 0);
		printf((char*)pshm);
		
		shmdt(pshm);
		shmctl(shmid, IPC_RMID, 0);
		
		
	}

	return 0;
}
