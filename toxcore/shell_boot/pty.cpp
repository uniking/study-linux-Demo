#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>

#include <tox/tox.h>

#include <string>
using namespace std;

pthread_t ntid;

typedef struct tox_pty {
    Tox *tox;
    uint32_t friend_number;
    int master;
} TOX_PTY;

uint32_t g_friend_number = 0;

void * thr_fn(void* arg)
{
	printf("pty thread\n");
	TOX_PTY* tw = (TOX_PTY*)arg;

        while (tox_friend_get_connection_status(tw->tox, tw->friend_number, 0)) {
	    int ret = 0;
            uint8_t buf[TOX_MAX_MESSAGE_LENGTH];
            ret = read(tw->master, buf, sizeof(buf));

            if (ret <= 0)
                continue;

            tox_friend_send_message(tw->tox, tw->friend_number, TOX_MESSAGE_TYPE_NORMAL, buf, ret, 0);

	    sleep(1);
        }
	

	free(arg);

	return NULL;
}

void ptytunnel(Tox *tox, uint32_t friend_number, int master)
{
	if(friend_number == g_friend_number)
		return;

	g_friend_number = friend_number;

	TOX_PTY* tw = (TOX_PTY*)malloc(sizeof(TOX_PTY));
	tw->tox = tox;
	tw->friend_number = friend_number;
	tw->master = master;

	pthread_t ntid = 0;
	pthread_create(&ntid, NULL, thr_fn, tw);
}
