#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socketapi.h"

#define Ip_Addr "127.0.0.1"
#define Port    5001
#define BUFSIZE 1024

char sends[BUFSIZE] = "\0";

int main(void)
{
    char * recvmsg = NULL;
    int sockfd = -1;
    sockfd = Initclient(Ip_Addr,Port);
    if(sockfd == -1)
    {
        fprintf(stderr, "socket error:%s\a\n", strerror(errno));
        exit(1);
    }
    //"c "
    memcpy(sends,"c",strlen("c")+1);
    //memcpy(send+2+strlen(Ip_Addr)+1,argv[2],strlen(argv[2)+1);
    printf("sendmsg is %s\n",sends);
    SendData(sockfd, sends);
    while(1)
    {
        recvmsg = RecvData(sockfd);
        printf("recvmsg is %s\n",recvmsg);
        if(strcmp(recvmsg,"over") == 0)
            break;
        SendData(sockfd,"ok");
    }
    close(sockfd);
}
    
