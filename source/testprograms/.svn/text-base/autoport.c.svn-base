/* ex3.c*/
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#define IP "127.0.0.1"

int main()
{
        char ip[50] ="\0";
	int sockfd;
        struct in_addr in;
	socklen_t len;
	struct sockaddr_in addr, raddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("create socket failed!\n");
		return -1;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(0);
	addr.sin_addr.s_addr=inet_addr(IP);
	if(bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))<0)
	{
		printf("bind socket failed!\n");
		return -2;
	}
	len=sizeof(raddr);
	if(getsockname(sockfd,(struct sockaddr *)&raddr,&len)<0)
	{
		printf("getsockname failed!\n");
		return -3;
	}
        //in.s_addr = ntohl(raddr.sin_addr.s_addr);
        strcpy(ip,inet_ntoa(raddr.sin_addr.s_addr));
        printf("------------------------------------------\n");
        printf("ip is %s,port is %d\n",ip,ntohs(raddr.sin_port));
	return 0;
}

