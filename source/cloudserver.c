#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "serversocket.h"
#include "cloudserver.h"
#define sIp_Addr "127.0.0.1"
#define sPort    5001
#define BUFSIZE 1024

char sendmsgs[BUFSIZE] = "s ";

int ServerInfo(char * ServerId,char * Ip_Addr,char * Port)
{
    char * recvmsg = NULL;
    int sockfd = -1;
    char * sendtmp;
    int sizelong;
    printf("ServerId is %s Ip_Addr is %s Port is %s\n",ServerId,Ip_Addr,Port);
    sockfd = Initclient(sIp_Addr,sPort);
    if(sockfd == -1)
    {
        fprintf(stderr, "socket error:%s\a\n", strerror(errno));
        exit(1);
    }
    //"s ip port"
    sizelong = 1;
    memcpy(sendmsgs+sizelong," ",2); 
    sizelong++;
    memcpy(sendmsgs+sizelong,ServerId,strlen(ServerId)+1);
    sizelong = sizelong + strlen(ServerId);
    memcpy(sendmsgs+sizelong," ",2);
    sizelong++;
    memcpy(sendmsgs+sizelong,Ip_Addr,strlen(Ip_Addr)+1);
    sizelong = sizelong + strlen(Ip_Addr);
    memcpy(sendmsgs+sizelong," ",2);
    sizelong++;
    memcpy(sendmsgs+sizelong,Port,strlen(Port)+1);
    printf("sendmsgs is %s\n",sendmsgs);
    SendData(sockfd, sendmsgs);
    recvmsg = RecvData(sockfd);
    printf("recvmsg is %s\n",recvmsg);
    close(sockfd);
    return 0;
}
    
    
int ServerCloseinfo(char * ServerId)
{
    char * recvmsg = NULL;
    int sizelong;
    int sockfd = -1;
    sockfd = Initclient(sIp_Addr,sPort);
    if(sockfd == -1)
    {
        fprintf(stderr, "socket error:%s\a\n", strerror(errno));
        exit(1);
    }
    //"s close ServerId"
    sizelong = 1;
    memcpy(sendmsgs+sizelong," ",2);
    sizelong ++; 
    memcpy(sendmsgs+sizelong,"close",strlen("close")+1);
    sizelong = sizelong + strlen("close");
    memcpy(sendmsgs+sizelong," ",2);
    sizelong ++;
    memcpy(sendmsgs+sizelong,ServerId,strlen(ServerId)+1);
    printf("sendmsgs is %s\n",sendmsgs);
    SendData(sockfd,sendmsgs);
    recvmsg = RecvData(sockfd);
    printf("recvmsg is %s\n",recvmsg);
    close(sockfd);
    return 0;
}
    
    