#include "goods_pool.hpp"
#include <errno.h>

int CGoodsPool::set_semvalue()
{
	union semun sem_union;
	sem_union.val = 0;
	if(semctl(m_sem_id, 0, SETVAL, sem_union) == -1)
	{
		printf("set_semvalue error\n");
		return 1;
	}

	return 0;
}
int CGoodsPool::semaphore_p()
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;

    while(semop(m_sem_id, &sem_b, 1) == -1)
    {
        if(errno == EINTR)
                continue;// interupted system call错误，
            else
        {
            printf("semaphore_p error %m\n");
                exit(-1);
        }
    }

	return 0;
}

int CGoodsPool::semaphore_v()
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;

    while(semop(m_sem_id, &sem_b, 1) == -1)
    {
        if(errno == EINTR)
                continue;
            else
        {
            printf("semaphore_p error %m\n");
                exit(-1);
        }
    }

	return 0;
}

// key 1234
CGoodsPool::CGoodsPool(int key)
{
  m_sem_id = 0;
  m_sem_id = semget((key_t)key, 1, 0666 | IPC_CREAT);
  set_semvalue();
  pthread_mutex_init(&m_lock, NULL);
}

CGoodsPool::~CGoodsPool()
{
}

void CGoodsPool::put_goods(GOODS goods)
{
  pthread_mutex_lock(&m_lock);
  m_pool.push_back(goods);
  pthread_mutex_unlock(&m_lock);
  
  semaphore_v();
}

GOODS CGoodsPool::get_goods()
{
  semaphore_p();
  
  pthread_mutex_lock(&m_lock);
  GOODS tmp = m_pool.front();
  m_pool.pop_front();
  pthread_mutex_unlock(&m_lock);
    
  return tmp;
}
