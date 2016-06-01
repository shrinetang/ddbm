/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  socketapi.c                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  socketapi                              */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/26                             */
/*  DESCRIPTION           :  the client socket                      */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/26
 *
 */
 
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
#define BUFSIZE 1024
//#define Ip_Addr "127.0.0.1"
//#define Port    5001

char sendbuf[BUFSIZE];                     
char recvbuf[BUFSIZE];
int sendlen = 0;
int recvlen = 0;
int nbytes = -1;

/* apply socket and connect with server */
int Initclient(char * Ip_Addr,int Port)
{
    int sockfd;
    struct sockaddr_in server_addr;
    memset(sendbuf, 0, BUFSIZE);
    memset(recvbuf, 0, BUFSIZE);
    sendlen = 0;
    recvlen = 0;
    nbytes = -1;
    //apply socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error:%s\a\n", strerror(errno));
        exit(1);
    }
    //printf("new sockfd is %d\n",sockfd);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    server_addr.sin_addr.s_addr = inet_addr(Ip_Addr);
    //client apply to connect
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "connect error:%s\a\n", strerror(errno));
        exit(1);
    }
    //printf("connect success\n");
    //connect success accpet server's welcome
    RecvData(sockfd);
    return sockfd;
}

/* initclient and openremoteservice */
int OpenRemoteService(char * Ip_Addr,int Port) 
{
    int sockfd = -1;
    sockfd = Initclient(Ip_Addr,Port);
    printf("sock_id is %d\n",sockfd);
    if(sockfd == -1)
    {
        printf("Initclient error!\n");
        exit(1);
    }
    return sockfd;
}

/* close sockfd */
int CloseRemoteService(int sockfd)
{
    close(sockfd);
    return 0;
}

/* send Data */
int SendData(int sockfd, char * sendmsg)
{
    nbytes = -1;
    int size = 0;
    size = strlen(sendmsg)+1;
    //printf("size of sendmsg is%d\n",size);
    memcpy(sendbuf,sendmsg,strlen(sendmsg)+1);
    nbytes = send(sockfd, sendbuf, strlen(sendbuf)+1,0);
    if(nbytes == -1)
    {
        fprintf(stderr, "recv error:%s\n", strerror(errno));
        exit(1);
    }
    //printf("send success!\n");
    sendbuf[nbytes] = '\0';
    //printf("send msg is %s\n",sendbuf);
    //printf("nbytes is %d\n",nbytes);
    return nbytes;
}

/* receive data */
char * RecvData(int sockfd)
{
    nbytes = -1;
    //memcpy(sendbuf,sendmsg,sizeof(sendmsg)+1);
    nbytes = recv(sockfd, recvbuf, sizeof(recvbuf),0);
    if(nbytes == -1)
    {
        fprintf(stderr, "recv error:%s\n", strerror(errno));
        exit(1);
    }
    recvbuf[nbytes] = '\0';
    printf("server:%s\n",recvbuf);
    return recvbuf;
}

/*
int main()
{
    int sockfd = -1;
    char dbname[30] = "mydbhdbfdfds";
    
    SendData(sockfd,dbname);
    RecvData(sockfd);
    return 0;
}
*/
