#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>

//按键模拟，按键包含按下和松开两个环节
void simulate_key(int fd, int kval)
{
	struct input_event event;
	gettimeofday(&event.time, 0);

	//按下kval键
	event.type = EV_KEY;
	event.value = 1;
	event.code = kval;
	write(fd, &event, sizeof(event));

	//同步，也就是把它报告给系统
	event.type = EV_SYN;
	event.value = 0;
	event.code = SYN_REPORT;
	write(fd, &event, sizeof(event));

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, 0);

	//松开kval键
	event.type = EV_KEY;
	event.value = 0;
	event.code = kval;
	write(fd, &event, sizeof(event));

	//同步，也就是把它报告给系统
	event.type = EV_SYN;
	event.value = 0;
	event.code = SYN_REPORT;
	write(fd, &event, sizeof(event));
}

//鼠标移动模拟
void simulate_mouse(int fd, int rel_x, int rel_y)
{
	struct input_event event;
	gettimeofday(&event.time, 0);

	//x轴坐标的相对位移
	event.type = EV_REL;
	event.value = rel_x;
	event.code = REL_X;
	write(fd, &event, sizeof(event));

	//y轴坐标的相对位移
	event.type = EV_REL;
	event.value = rel_y;
	event.code = REL_Y;
	write(fd, &event, sizeof(event));

	//同步
	event.type = EV_SYN;
	event.value = 0;
	event.code = SYN_REPORT;
	write(fd, &event, sizeof(event));
}

int main(int argc, char **argv)
{
	int fd_mouse = -1;
	int fd_kbd = -1;
	int i = 0;

    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }
    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");

	// 请保证该设备节点有写的权限
	fd_kbd = open("/dev/input/event3", O_RDWR);
	if(fd_kbd <= 0) {
	    printf("Can not open keyboard input file\n");
	    return -1;
	}

	// 请保证该设备节点有写的权限
	fd_mouse = open("/dev/input/event4", O_RDWR);
	if(fd_mouse <= 0) {
	    printf("Can not open mouse input file\n");
	    return -1;
	}
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    char buff[512];
    struct sockaddr_in clientAddr;
    int n;
    socklen_t len = sizeof(clientAddr);
    while (1)
    {
        n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);
        if (n>0)
        {
            buff[n] = 0;
/*
            printf("%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
            n = sendto(sock, buff, n, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
            if (n < 0)
            {
                perror("sendto");
                break;
            }
*/
		if(0 == memcmp(buff, "up", 2))
		{
			simulate_key(fd_kbd, KEY_UP);
		}
		else if(0 == memcmp(buff, "down", 4))
		{
			simulate_key(fd_kbd, KEY_DOWN);
		}
		else if(0 == memcmp(buff, "left", 4))
		{
			simulate_key(fd_kbd, KEY_LEFT);
		}
		else if(0 == memcmp(buff, "right", 5))
		{
			simulate_key(fd_kbd, KEY_RIGHT);
		}
		else if(0 == memcmp(buff, "ok", 2))
		{
			simulate_key(fd_kbd, KEY_ENTER);
		}
		else if(0 == memcmp(buff, "sleep", 5))
		{
			simulate_key(fd_kbd, KEY_L);
		}
		else if(0 == memcmp(buff, "return", 6))
		{
			simulate_key(fd_kbd, KEY_ESC);
		}
		else
		{
		}
        }
        else
        {
            perror("recv");
            break;
        }
    }
    return 0;
}
