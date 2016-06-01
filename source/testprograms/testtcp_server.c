#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/type.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAXSIZE 1024
#define MAX_CONNECT_QUEUE 20
#define Ip_Addr 127.0.0.1
#define Port    5001
#define BUFSIZE 1024

int TCP_server()
{
    int sockfd,new_fd;
    struct sockaddr_in server_addr;     /*server socket struct*/
    struct sockaddr_in client_addr;     /*store client socket struct*/
    char sendbuf[BUFSIZE];                     
    char recvbuf[BUFSIZE];
    int sendlen = 0;
    int recvlen = 0;
    int sin_size = 0;
    int nbytes = 0;
    char buf[MAXSIZE];
    //server apply socket
    memset(sendbuf, 0, BUFSIZE);        /*clear the sendbuf*/
    memset(recvbuf, 0, BUFSIZE);        /*clear the recvbuf*/
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    //server init sockaddr
    bzero(&server_addr, sizeof(struct sockaddr_in));    //clear server sockaddr
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  htonl(Ip_Addr);      //set ip
    server_addr.sin_port = htons(Port);                 //set port
    //bind
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
    //process the whole apply connection
    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);  //save the client scoketsize
        //accept a reply
        if(new_fd = accpet(sockfd, (struct sockaddr *)(&client_addr),&sin_size)) == -1)
        {
            fprintf(stderr,"accept error:%s\n\a", strerror(errno));
            exit(1);
        }
        fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr));
        //TCP connection have already built
        printf("Connected successful,now recv msg:\n");
        //remind client connection success
        strcpy( sendbuf, "welcome to ustc:you can put command");
        sendlen = strlen(sendbuf);
        send(new_fd, sendbuf, sendlen,0); 
        //recv data
        if(nbytes = recv(new_fd, recvbuf, sizeof(recvbuf), 0))
        {
            fprintf(stderr, "recv error:%s\n", strerror(errno));
            exit(1);
        }
        recvbuf[nbytes] = '\0';
        printf("I have received:%s\n",recvbuf);
        close(new_fd);
    }
    close(sockfd);
    return 0;
}

       
int main(void)
{
    TCP_server();
    return 0;
}

