/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  masterserver.c                         */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  masterserver                           */
/*  MODULE NAME           :  masterserver                           */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/1/5                               */
/*  DESCRIPTION           :  masterserver for the server's cluster  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu,2012/1/5
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define MAXSIZE 1024
#define MAX_CONNECT_QUEUE 20
#define BUFSIZE 1024
#define MAXCONNECT 1024

#ifdef _DEBUG
    #define debug_print(s)  printf s
#else
    #define debug_print(s)
#endif

char sendbuf[BUFSIZE];                     
char recvbuf[BUFSIZE];
int sendlen = 0;
int recvlen = 0;    
int sin_size = 0;   
int sockfd = -1;
int new_fd = -1;

/*server apply socket and bind*/
int ServerPrepare(char * Ip_Addr,int Port)
{
    int sockfd = -1;
    struct sockaddr_in server_addr;     
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    //server init sockaddr
    bzero(&server_addr, sizeof(struct sockaddr_in));    //clear server sockaddr
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  inet_addr(Ip_Addr);      //set ip
    server_addr.sin_port = htons(Port);                 //set port
    if(bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "bind error:%s\n\a", strerror(errno));
        exit(1);
    }
    //listen 5 socketport
    if(listen(sockfd,MAX_CONNECT_QUEUE) == -1) 
    {
        fprintf(stderr, "listen error:%s\n\a", strerror(errno));
        exit(1);
    }
    return sockfd;
}

/* send data */
int SendData(int sockfd, char * sendmsg)
{
    int nbytes = -1;
    int size = 0;
    size = strlen(sendmsg)+1;
    //printf("size of sendmsg is%d\n",size);
    memset(sendbuf, 0, BUFSIZE);       
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

/*accpet the connect and return new_fd*/
int ServerOpen(int sockfd)
{
     struct sockaddr_in client_addr;     
     sin_size = sizeof(struct sockaddr_in);  //save the client scoketsize   
     //accept a reply
     new_fd = accept(sockfd, (struct sockaddr *)(&client_addr),&sin_size);
     if(new_fd == -1)
     {
        fprintf(stderr,"accept error:%s\n\a", strerror(errno));
        exit(1);
     }
     debug_print(("Connected successful,now recv msg:\n"));
     SendData(new_fd,"welcome");
     return new_fd;
}

/* receive data */
char * RecvData(int sockfd)
{
    int nbytes = -1;
    memset(recvbuf, 0, BUFSIZE);        /*clear the recvbuf*/
    //memcpy(sendbuf,sendmsg,sizeof(sendmsg)+1);
    nbytes = recv(sockfd, recvbuf, sizeof(recvbuf),0);
    if(nbytes == -1)
    {
        fprintf(stderr, "recv error:%s\n", strerror(errno));
        exit(1);
    }
    recvbuf[nbytes] = '\0';
    printf("recved:%s\n",recvbuf);
    return recvbuf;
}

/* for storing the sockfd and their info */
typedef struct Savesockfd
{
    int new_fd;         
    int client;        //if 1 it's client msg
    int server;        //if 1 it's server msg
    int sindex;        //save the serverinfo [index]
}savesockfd;

savesockfd Index[MAXCONNECT];

typedef struct SInfo
{
    int newfd;
    char sid[5];
    char sip[50];
    char sport[10];
}sinfo;

sinfo Record[20];

char sendinfo[1024] = "\0";

/* send the server info to client */    
int SendInfo(int new_fd)
{
    int i,size;
    for(i = 0; i<20&&(Record[i].newfd!=0); i++)
    {
        size = 0;
        memcpy(sendinfo,Record[i].sid,strlen(Record[i].sid)+1);
        size = strlen(Record[i].sid);
        memcpy(sendinfo+size," ",2);
        size ++;
        memcpy(sendinfo+size,Record[i].sip,strlen(Record[i].sip)+1);
        size = size + strlen(Record[i].sip);
        memcpy(sendinfo+size," ",2);
        size ++;
        memcpy(sendinfo+size,Record[i].sport,strlen(Record[i].sport)+1);
        printf("SendInfo %d is %s\n",i,sendinfo);
        SendData(new_fd,sendinfo);
        RecvData(new_fd);
    }
    SendData(new_fd,"over");
    printf("SendInfo %d is %s\n",i,"over");
    return 0;
}

int main(int argc,char *argv[])
{
    char * recvbuf = NULL;
    int size,i,j;
    int sockfd = -1;
    int new_fd = -1;
    char tmpcmd[20] = "\0";
    char tmpid[10] = "\0";
    char * Ip_Addr;
    int Port;
    Ip_Addr = argv[1];
    Port = atoi(argv[2]);
    i = 0;
    j = 0;
    sockfd = ServerPrepare(Ip_Addr,Port);
    while(1)
    {
        new_fd = ServerOpen(sockfd);
        Index[i].new_fd = new_fd;     //record new_fd
        recvbuf = RecvData(new_fd);
        //printf(" %d new_fd is %d\n",i,Index[i].new_fd);
        debug_print(("%d recvbuf is %s\n",i,recvbuf));
        if(recvbuf[0] == 'c')         //recv a client connect and send server info to client
        {
            Index[i].client = 1;
            debug_print(("%d msg is client msg\n",i));
            SendInfo(new_fd);
        }
        else if(recvbuf[0] == 's')    //recv a server connect and store the server info
        {
            Record[j].newfd = new_fd;
            Index[i].server = 1;
            Index[i].sindex = j;         //save serverinfo's index
            debug_print(("%d msg is server msg\n",i));
            sscanf(recvbuf,"s %s %s %s",Record[j].sid,Record[j].sip,Record[j].sport);
            debug_print(("Record[%d].sip is %s and Port is %s\n",j,Record[j].sip,Record[j].sport));
            SendData(new_fd,"ok");  
            j++;          
        }
        else
        {
            debug_print(("recv a wrong msg,do nothing\n"));
        }
        i++;
    }
    CloseServer();
    return 0;
}
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
 
    
