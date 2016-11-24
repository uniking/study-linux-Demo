#include "goods_pool.hpp" 

void *
thr_consumer(void* arg)
{
	CGoodsPool* pAb = (CGoodsPool*)arg;
	printf("c:%d\n",pAb->get_goods());

	pAb = (CGoodsPool*)arg;
	printf("c:%d\n",pAb->get_goods());

	pAb = (CGoodsPool*)arg;
	printf("c:%d\n",pAb->get_goods());

	pAb = (CGoodsPool*)arg;
	printf("c:%d\n",pAb->get_goods());

	printf("c:exit\n");
}

void *
thr_producer(void* arg)
{
	CGoodsPool* pAb = (CGoodsPool*)arg;
	pAb->put_goods(135);
	pAb->put_goods(136);
	pAb->put_goods(137);
}

int main()
{
	pthread_t ntid;
	CGoodsPool ab(1234);

	pthread_create(&ntid, NULL, thr_producer, &ab);

	sleep(2);
	pthread_create(&ntid, NULL, thr_consumer, &ab);

	sleep(5);

}
