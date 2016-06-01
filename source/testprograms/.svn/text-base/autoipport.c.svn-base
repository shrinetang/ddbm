/* ex3.c*/
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/if.h>

//#define IP "127.0.0.1"
char ipptr[50] ="\0";
int getlocalhostip();
char sPort[20] = "\0";
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
	getlocalhostip();
	printf("ip is %s\n",ipptr);
	addr.sin_addr.s_addr=inet_addr(ipptr);
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
    strcpy(ip,inet_ntoa(raddr.sin_addr));
    printf("------------------------------------------\n");
    printf("ip is %s,port is %d\n",ip,ntohs(raddr.sin_port));
    sprintf(sPort,"%d",ntohs(raddr.sin_port));
    printf("sPort is %s\n",sPort);
	return 0;
}

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
                    printf("----------------------------------------------\n");
                    printf("ip is %s\n",ipptr);
                    break;
                }
              
            }
   
        }
    close (fd);
    }
    return 0;
}
