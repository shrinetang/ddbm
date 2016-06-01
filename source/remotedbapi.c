/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  remotedbapi.c                          */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  remotedbapi                            */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/12/15                             */
/*  DESCRIPTION           :  database application remoteaopi        */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu,2012/12/15
 *
 */

#include "remotedbapi.h"
#include "socketapi.h"
#include <tcutil.h>
#include <tchdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define  SENDSIZE 128
//int sockfd = -1;

/*change int to string */
int my_itoa(int val, char* buf)
{
    const int radix = 10;
    char* p;
    int a;        //every digit
    int len;
    char* b;    //start of the digit char
    char temp;
    p = buf;
    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    b = p;
    do
    {
        a = val % radix;
        val /= radix;
        *p++ = a + '0';

    } while (val > 0);
    len = (int)(p - buf);
    *p-- = 0;
    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    } while (b < p);
    return len;
}

/* from the result which is sended by server to get index of db*/
int GetIndex(char * buf)
{
    int i,j;
    int sum = 0;
    int size = -1;
    size = strlen(buf)-1;
    if(buf[size]<48||buf[size]>57||buf[0] == 'o'||buf[0] == 'O')
        return -1;
    for(i = size ,j = 1 ; (buf[i] != ' ')&& (i > 0) ; i--)
    {
        sum += (buf[i]-48) *j;
        j = j * 10;
    }
    buf[i] = '\0';
    //printf("get index is %d\n",sum);
    return sum;
}

/*create a database object*/
int createDB(char * buf,char * Ip_Addr,int Port,int * sock)
{
    int sockfd = -1;
    int dbindex = -1;
    //int * index; 
    int sendsize = -1;
    char sendtmp[100] = "\0";
    char *recvtmp = NULL;
    memcpy(sendtmp,buf,strlen(buf)+1);
    //sendtmp[sendsize] = '\0';
    sockfd = OpenRemoteService(Ip_Addr,Port);
    *sock = sockfd;
    //printf("in createDB,sockfd is %d\n",sockfd);   
    if(SendData(sockfd,sendtmp) == -1)
    {
        printf("SendData error!\n");
        exit(1);
    }
    printf("now send %s\n",sendtmp);
    recvtmp = RecvData(sockfd);
    dbindex = GetIndex(recvtmp);
        //printf("dbindex return is %d\n",dbindex);   
    return dbindex;
}

/*put the key and value into the database*/
int setKeyValue(char * buf,int dbindex,int sockfd)
{
    char sendtmp[100] ="\0";
    char * recvtmp = NULL;
    int size = 0;
    printf("set dbindex is %d and sockfd is %d\n",dbindex,sockfd);
    memcpy(sendtmp,buf,strlen(buf)+1);   
    //sprintf(sendtmp,"s dsd %d",index);
    size = my_itoa(dbindex,sendtmp+strlen(sendtmp));
    sendtmp[strlen(sendtmp)+size] = '\0';
    //printf("set msg you want is %s\n",sendtmp);
    if(SendData(sockfd,sendtmp) == -1)
    {
        printf("SendData error!\n");
        exit(1);
    }
    recvtmp = RecvData(sockfd);
    printf("%s\n",recvtmp);      
    return 0;
}

/*get the value across the key,and store the result into the pointer of pvalue*/
int getKeyValue(char * buf,int dbindex,int sockfd)
{
    char sendtmp[100] ="\0";
    char * recvtmp = NULL;
    int size = 0;
    memcpy(sendtmp,buf,strlen(buf)+1);
    size = my_itoa(dbindex,sendtmp+strlen(sendtmp));
    sendtmp[strlen(sendtmp)+size] = '\0'; 
    //printf("get msg you want to sends %s\n",sendtmp);
    if(SendData(sockfd,sendtmp) == -1)
    {
        printf("SendData error!\n");
        exit(1);
    }
    recvtmp = RecvData(sockfd);
    printf("get:%s\n",recvtmp);      
    return 0;
}


/*release the db object*/
int deleteDB(int dbindex,int sockfd)
{
    char sendtmp[100] ="close ";
    char * recvtmp = NULL;
    int size = 0;
    if(sockfd == -1)
    {
        printf("you must connect server first!\n");
        exit(1);
    }
    size = my_itoa(dbindex,sendtmp+strlen(sendtmp));
    sendtmp[strlen(sendtmp)+size] = '\0'; 
    //printf("delete msg you want to send is %s\n",sendtmp);
    if(SendData(sockfd,sendtmp) == -1)
    {
        printf("SendData error!\n");
        exit(1);
    }
    recvtmp = RecvData(sockfd);
    //RecvData(int sockfd);
    printf("Recv is %s\n",recvtmp);     
    return 0;
}

/*delete the keyValue of db object*/
int deleteKeyValue(char * buf,int dbindex,int sockfd)
{
    char sendtmp[100] ="\0";
    char * recvtmp = NULL;
    int size = 0;
    memcpy(sendtmp,buf,strlen(buf)+1);
    size = my_itoa(dbindex,sendtmp+strlen(sendtmp));
    sendtmp[strlen(sendtmp)+size] = '\0'; 
    //printf("delete msg you want to sends %s\n",sendtmp);
    if(SendData(sockfd,sendtmp) == -1)
    {
        printf("SendData error!\n");
        exit(1);
    }
    recvtmp = RecvData(sockfd);
    printf("Recv is %s\n",recvtmp);      
    return 0;
    
}
