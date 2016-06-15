#include "Queue.h"
#include <linux/vmalloc.h>
#include <linux/mutex.h>

PTCP_DATA queue_buffer = NULL;
long queue_header = 0;
long queue_tail = 0;
long queue_capacity = 0;
//static struct mutex queue_mutex;
static DEFINE_SPINLOCK(queue_spin);
//spinlock_t queue_spin=SPIN_LOCK_UNLOCKED;
long add_index(long index)
{
	return (index+1)%queue_capacity;
}

int init_tcp_queue_sx(long capacity)
{
	int ret;
	queue_capacity = capacity+1;
	queue_buffer = vmalloc(queue_capacity * sizeof(TCP_DATA));
	if(queue_buffer == NULL)
		printk("queue buffer null, vmalloc error\n");
	queue_tail = queue_header = 0;

//	mutex_init(&queue_mutex);
	spin_lock_init(&queue_spin);

	return QUEUE_OK;
}

void uninit_tcp_queue_sx()
{
	if(queue_buffer)
		vfree(queue_buffer);
}

int put_data(PTCP_DATA tcp_data)
{
	printk("put_data lock\n");
//	mutex_lock(&queue_mutex);
	spin_lock(&queue_spin);

	if(add_index(queue_tail) == queue_header)
	{
//		mutex_unlock(&queue_mutex);
		spin_unlock(&queue_spin);
		printk("put_data queue full\n");
		return QUEUE_FULL;
	}

	memcpy(&queue_buffer[queue_tail], tcp_data, sizeof(TCP_DATA));

	queue_tail = add_index(queue_tail);

//	mutex_unlock(&queue_mutex);
	spin_unlock(&queue_spin);
	printk("put_data unlock\n");
	return QUEUE_OK;
}

int get_data(PTCP_DATA tcp_data)
{
//	mutex_lock(&queue_mutex);
	spin_lock(&queue_spin);

	if(queue_header == queue_tail)
	{
//		mutex_unlock(&queue_mutex);
		spin_unlock(&queue_spin);
		return QUEUE_EMPTY;
	}

	memcpy(tcp_data, &queue_buffer[queue_header], sizeof(TCP_DATA));

	queue_header = add_index(queue_header);

//	mutex_unlock(&queue_mutex);
	spin_unlock(&queue_spin);
	return QUEUE_OK;
}
