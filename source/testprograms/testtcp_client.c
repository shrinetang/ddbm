#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/type.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFSIZE 1024
#define Ip_Addr 127.0.0.1
#define Port    5001


int TCP_client(void)
{
    int sockfd;
    int cnt = 1;
    char sendbuf[BUFSIZE];                     
    char recvbuf[BUFSIZE];
    struct sockaddr_in server_addr;
    memset(sendbuf, 0, BUFSIZE);
    memset(recvbuf, 0, BUFSIZE);
    int sendlen = 0;
    int recvlen = 0;
    int nbytes = 0;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error:%s\a\n", strerror(error));
        exit(1);
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    server_addr.sin_addr = inet_addr(Ip_Addr);
    //client apply to connect
    while(cnt)
    {
    printf("if you don't want to send msg ,you can input 0 to quit");
    scanf("%d",cnt);
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "connect error:%s\a\n", strerror(errno));
        exit(1);
    }
    //connect success
    if(nbytes = recv(sockfd, recvbuf, sizeof(recvbuf),0)
    {
        fprintf(stderr, "recv error:%s\n", strerror(error));
        exit(1);
    }
    recvbuf[nbytes] = '\0';
    printf("server send :%s\n",recvbuf);
    printf("you can put msg that you want send to server:\nMsg :\n");
    scanf("%s",sendbuf);
    if(nbytes = send(sockfd, sendbuf, sizeof(sendbuf),0)
    {
        fprintf(stderr, "recv error:%s\n", strerror(error));
        exit(1);
    }
    printf("send success!\n");
    }
    close(sockfd);
    return 0;
}

int main(void)
{
    TCP_client();
    return 0;
}
