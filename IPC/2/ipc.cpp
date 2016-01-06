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
#include <sys/sem.h>

#define FIFO_NAME "/tmp/my_fifo"

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

int set_semvalue(int sem_id)
{
	union semun sem_union;
	sem_union.val = 1;
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		printf("set_semvalue error\n");
		return 1;
	}

	return 0;
}
int semaphore_p(int sem_id)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		printf("semaphore_p error\n");
		return 1;
	}

	return 0;
}

int semaphore_v(int sem_id)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		printf("semaphore_v error\n");
		return 1;
	}

	return 0;
}

int main()
{
	int pid;
	if((pid=fork()) < 0)
	{
		printf("fork error\n");
	}
	else if(pid ==0)
	{
		int sem_id = 0;
		sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
		set_semvalue(sem_id);
	
		//获取一个资源
		semaphore_p(sem_id);
		printf("father get resource\n");
		sleep(3);
		printf("father to release resource\n");
		semaphore_v(sem_id);
		
	}
	else
	{
		//等待父进程处理也下资源
		sleep(1);
		int sem_id = 0;
		sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

		//获取一个资源
		printf("son to get resource\n");
		semaphore_p(sem_id);
		printf("son get resource\n");
		sleep(1);
		printf("son to release resource\n");
		semaphore_v(sem_id);
		
	}
}
