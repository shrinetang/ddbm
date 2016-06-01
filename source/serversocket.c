/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  servernew.c                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server socket                          */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/1/10                              */
/*  DESCRIPTION           :  the serversocket                       */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/1/10
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
#include<sys/epoll.h>
#include<fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <sys/time.h>
#include "servercmd.h"
#define MAXSIZE 1024
#define MAX_CONNECT_QUEUE 20
#define MAX_CONNECT 20
#define BUFSIZE 1024
#define sIp_Addr "127.0.0.1"
#define sPort    5001
int sockfd,new_fd;
int epollfd = -1; 
int sin_size = 0;
struct sockaddr_in server_addr;     /*server socket struct*/
struct sockaddr_in client_addr;     /*store client socket struct*/
char recvbuf[BUFSIZE];
char sendbuf[BUFSIZE];
int sendlen = 0;
int recvlen = 0;
int nbytes = -1;
char ipptr[50] ="\0";
char ports[20] = "\0";
char sendmsgs[BUFSIZE] = "s ";

int SendData(int sockfd, char * sendmsg);
char * RecvData(int sockfd);

/* set epoll as a nonblock state */
int SetNonBlocking(int fd)
{
    int opts;
    opts = fcntl(fd,F_GETFL);
    if(opts < 0)
    {
        fprintf(stderr,"fcntl(sock,GETFL) Error,%s:%d\n", __FILE__,__LINE__);
        exit(1);
    }
    /* set non blocking */
    opts = opts|O_NONBLOCK;
    if(fcntl(fd,F_SETFL,opts) < 0)
    {
        fprintf(stderr,"fcntl(sock,SETFL,opts) Error,%s:%d\n", __FILE__,__LINE__);
        exit(1);
    }    
}      

/* send server info to masterserver */
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

/* get localip and useful port */
int getlocalhostip()
{
    int  MAXINTERFACES=16;
    long ip;
    int fd, intrface, retn = 0;
    struct ifreq buf[MAXINTERFACES]; ///if.h
    struct ifconf ifc; ///if.h
    ip = -1;
    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) //socket.h
    {
        ifc.ifc_len = sizeof buf;
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) //ioctl.h
        {
            intrface = ifc.ifc_len / sizeof (struct ifreq); 
            while (intrface-- > 0)
            {
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
                    ip=inet_addr( inet_ntoa( ((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr) );//types
                    strcpy(ipptr,inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                    //printf("ip is %s\n",ipptr);
                    break;
                }
              
            }
   
        }
    close (fd);
    }
    return 0;
}

/*server apply socket and bind*/
int ServerPrepare(char * ServerId)
{
    char ip[50] ="\0";
    socklen_t len;
    struct sockaddr_in raddr;
    struct epoll_event event;
    getlocalhostip();
    printf("ip is %s\n",ipptr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  inet_addr(ipptr);      //set ip
    server_addr.sin_port = htons(0);                 //set port
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    epollfd = epoll_create(MAX_CONNECT);
    SetNonBlocking(sockfd);
    event.data.fd = sockfd;
    event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event);
    if(bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "bind error:%s\n\a", strerror(errno));
        close(sockfd); 
        exit(1);
    } 
    len=sizeof(raddr);
    if(getsockname(sockfd,(struct sockaddr *)&raddr,&len)<0)
    {
	printf("getsockname failed!\n");
	return -1;
    }
    strcpy(ip,inet_ntoa(raddr.sin_addr));
    //printf("ip is %s,port is %d\n",ip,ntohs(raddr.sin_port));
    sprintf(ports,"%d",ntohs(raddr.sin_port));
    //printf("ports is %s\n",ports);
    ServerInfo(ServerId,ip,ports);
    if(listen(sockfd,MAX_CONNECT_QUEUE) == -1) 
    {
        fprintf(stderr, "listen error:%s\n\a", strerror(errno));
        exit(1);
    }
    return 0;
}

/*accpet the connect and return new_fd*/
int ServerOpen()
{
    int fd;
    while(1)
    {
        struct epoll_event event;
        fd = epoll_wait(epollfd,&event,1,-1);
        if(event.data.fd == sockfd)
        {
            sin_size = sizeof(struct sockaddr_in);  //save the client scoketsize
            //accept a reply
            new_fd = accept(sockfd, (struct sockaddr *)(&client_addr),&sin_size);
            if(new_fd == -1)
            {
                fprintf(stderr,"accept error:%s\n\a", strerror(errno));
                exit(1);
            }
            printf("Connected successful,now recv msg:\n");
            SendData(new_fd, "welcome to visit\n");
            printf("Connected after\n");
            SetNonBlocking(new_fd);
            event.data.fd = new_fd;
            event.events = EPOLLIN;
            epoll_ctl(epollfd,EPOLL_CTL_ADD,new_fd,&event);
        }    
        else if((event.events & EPOLLIN) && (event.events & EPOLLRDHUP))
        {
            close(event.data.fd);
        }
        else
        {
            return (int)event.data.fd;
        }
    }
    // return new_fd;
}

/* send Data */
int SendData(int sockfd, char * sendmsg)
{
    nbytes = -1;
    int size = 0;
    size = strlen(sendmsg)+1;
    //printf("size of sendmsg is%d\n",size);
    memset(sendbuf, 0, BUFSIZE);        /*clear the sendbuf*/
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
    RecvData(sockfd);
    return sockfd;
}

/* close server sockfd and close epoll */
int CloseServer()
{
    if(sockfd <= 0)
    {
        return -1;
    }
    close(sockfd);
    if(epollfd == -1)
    {
        return -1;
    }
    close(epollfd);
    return 0;  
}





    
