#include "goods_pool.hpp" 

void *
thr_consumer(void* arg)
{
   CGoodsPool* pAb = (CGoodsPool*)arg;
   printf("%d\n",pAb->get_goods());
}

void *
thr_producer(void* arg)
{
     CGoodsPool* pAb = (CGoodsPool*)arg;
     sleep(1);
     pAb->put_goods(135);
}

int main()
{
  pthread_t ntid;
  CGoodsPool ab(1234);
 pthread_create(&ntid, NULL, thr_consumer, &ab);
  pthread_create(&ntid, NULL, thr_producer, &ab);
  
  sleep(10);
}
