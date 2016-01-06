#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define MAXLINE 800
int port = 8000;

int main(void)
{
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    int listen_fd;
    int conn_fd;
    int sock_fd;
    int nready;
    int maxi;
    int max;
    int client[FD_SETSIZE];
    int address_size = sizeof(pin);
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i;
    int len;
    int n;
    int ret;

    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd)
    {
        perror("call to socket");
        exit(1);
    }
    n = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
    if (-1 == n)
    {
        perror("call to bind");
        exit(1);
    }
    n = listen(listen_fd, 20);
    if (-1 == n)
    {
        perror("call to listen");
        exit(1);
    }
    printf("Accepting connections...\n");

    while(1)
    {
        conn_fd = accept(listen_fd, (struct sockaddr *)&pin, &address_size);

        n = fork();
        if (-1 == n){
            perror("call to fork");
            exit(1);
        }

        else if (0 == n)
        {
            close(listen_fd);
            while(1)
            {
                memset(buf,'\0',MAXLINE);
readagain:
                ret = read(conn_fd,buf,MAXLINE);
                printf("I read %d Byte!\n",ret);

                if (-1 == ret){
                    if (errno == EINTR){
                        goto readagain;
                    }else{
                        perror("call to read");
                        exit(1);
                    }
                } else if (0 == ret){
                    printf("the other side has been closed.\n");
                    break;
                }


                printf("you ip is %s at port %d:%s\n",
                     inet_ntop(AF_INET, &pin.sin_addr,str,sizeof(str)),
                     ntohs(pin.sin_port),buf);

                len = strlen(buf);
                for (i = 0; i < len; i++)
                {
                    buf[i] = toupper(buf[i]);
                }

writeagain:
                ret = write(conn_fd, buf, len+1);
                printf("I write %d Byte!\n",ret);

                if (-1 == ret){
                    if (errno == EINTR){
                        goto writeagain;
                    } else {
                        perror("call to write!");
                        break;
                    }
                }
            }// end while


            ret = close(conn_fd);
            if (-1 == ret){
                perror("call close");
                return -1;
            }
            exit(0);
        }
    }
    return 0;
}
