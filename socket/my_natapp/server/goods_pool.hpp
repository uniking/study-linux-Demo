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

#include <list>
using namespace std;

#define FIFO_NAME "/tmp/my_fifo"

#define GOODS void*
//#define GOODS int
#define GOODS_POOL list< GOODS >

// for thread
class CGoodsPool
{
private:
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short *arry;
	};

	GOODS_POOL m_pool;
	int m_sem_id;
	pthread_mutex_t m_lock;
	int m_mutex_result;

	int semaphore_v();
	int semaphore_p();
	int set_semvalue();
public:
	CGoodsPool(int key);
	~CGoodsPool();

	GOODS get_goods();
	void put_goods(GOODS goods);
	int size();
};
