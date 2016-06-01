/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  remotedbapi.c                          */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  clouddbapi                             */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/12/29                             */
/*  DESCRIPTION           :  database application remoteaopi        */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu,2012/12/29
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include "remotedbapi.h"
#include "clouddbapi.h"
#include "taskqueue.h"
#include "socketapi.h"
#ifdef _DEBUG
    #define debug_print(s)  printf s
#else
    #define debug_print(s)
#endif

#define Ip_Addr "127.0.0.1"
#define Port    5001
#define BUFSIZE 1024
 
tQueue * p;
tQueueNode * ptask;
int Count = 0;
/* for temporary storing the serverinfo */
char tmpsid[20] = "\0";
char tmpsip[100] = "\0";
char tmpsport[20] ="\0";

/* use queue for storing the serverinfo */ 
tQueue * ServerLoad(char * sid,char * sip,char * sport)
{
    int i;
    tQueueNode * find = NULL;
    ptask = (tQueueNode *)malloc(sizeof(struct QueueNode));
    memcpy(ptask->pBuf,sip,strlen(sip)+1);
    ptask->bufsize = atoi(sport);
    debug_print(("ptask you want insert:ip is %s,port is %d\n", ptask->pBuf,ptask->bufsize));  
    InQueue(p,ptask);
    for(find = p->pHead,i = 0; ;find = find->pNext,i++)
    {
        //printf("queue %d is %s, %d\n",i,find->pBuf,find->bufsize);
        if(find == p->pTail)
            break;
    }
    Count = p->Sum;
    //printf("Sum is %d\n",p->Sum);
    return p;                                   
}

/* from the masterserver to get the serverinfo */
int GetServer(void)
{
    char sends[BUFSIZE] = "\0";
    char * recvmsg = NULL;
    int sockfd = -1;
    sockfd = Initclient(Ip_Addr,Port);
    if(sockfd == -1)
    {
        printf("sock error!\n");
        exit(1);
    }
    //"c "
    memcpy(sends,"c",strlen("c")+1);
    printf("sendmsg is %s\n",sends);
    SendData(sockfd, sends);
    while(1)
    {
        recvmsg = RecvData(sockfd);
        debug_print(("recvmsg is %s\n",recvmsg));
        if(strcmp(recvmsg,"over") == 0)
            break;
        else
        {
            sscanf(recvmsg,"%s %s %s",tmpsid,tmpsip,tmpsport);
            ServerLoad(tmpsid,tmpsip,tmpsport);
        }    
        SendData(sockfd,"ok");
    }
    close(sockfd);
}

/* to find the server which you want to communicate from the queuelist */
tQueueNode * ServerFind(int Sno)
{
    int i;
    tQueueNode * loc = NULL;
    if(Sno < 0)
    {
        printf("Sno error!\n");
        return NULL;
    }
    printf("Sno is %d\n",Sno);
    loc = p->pHead;
    for(i = 0 ; i < Sno; i++,loc = loc->pNext)
        ;
    if(loc == NULL)
    {
        printf("can't find Server!\n");
        return NULL;
    }
    //printf("loc index is %d & sockfd is %d\n",loc->dbindex,loc->sockfd);
    return loc;
}

/*get server from the masterserver and load server,then connect with all server and create db*/
int CloudDbcreate(char * buf)
{
    int i = 1;
    p = (tQueue *)malloc(sizeof(struct Queue)); 
    InitQueue(p);
    GetServer();
    tQueueNode * find = NULL;
    //connect with all server
    for(find = p->pHead; ;find = find->pNext)
    {
        find->dbindex = createDB(buf,find->pBuf,find->bufsize,&find->sockfd);  //store index
        //printf("createDB index is %d & sockfd is %d\n",find->dbindex,find->sockfd);
        if(find == p->pTail)
            break;
    }
    return find->dbindex;
}

/*find the serverinfo and set key-vlaue*/ 
int CloudDbsetKeyValue(char * buf)
{
    tQueueNode * tmp;
    int snum = 0 ;
    char value[100] = "\0";
    int key;
    sscanf(buf,"set %d %s",&key,value); 
    debug_print(("sscanf key is %d value is %s\n",key,value));
    snum = key%Count;
    tmp = ServerFind(snum);
    //printf("tmp dbindex is %d,tmp sockfd is %d\n",tmp->dbindex,tmp->sockfd);
    setKeyValue(buf,tmp->dbindex,tmp->sockfd);
    return 0;
}

/*find the serverinfo and get key-vlaue*/ 
int CloudDbgetKeyValue(char * buf)
{
    tQueueNode * tmp;
    int snum = 0 ;
    int key = 0;
    sscanf(buf,"get %d",&key); 
    debug_print(("sscanf key is %d\n",key));
    snum = key%Count;
    tmp = ServerFind(snum);
    getKeyValue(buf,tmp->dbindex,tmp->sockfd);
    return 0;
}

/*close all the connection with server*/ 
int CloudDbclose()
{
    tQueueNode * find = NULL;
    //disconnect with all server
    for(find = p->pHead; ;find = find->pNext)
    {
        find->dbindex = deleteDB(find->dbindex,find->sockfd); 
        if(find == p->pTail)
            break;
    }
    return 0;
}

/*find the serverinfo and delete key-vlaue*/ 
int CloudDbdeleteKeyValue(char * buf)
{
    tQueueNode * tmp;
    int snum = 0 ;
    int key = 0;
    sscanf(buf,"delete %d",&key); 
    debug_print(("sscanf key is %d\n",key));
    snum = key%Count;
    tmp = ServerFind(snum);
    deleteKeyValue(buf,tmp->dbindex,tmp->sockfd);
    return 0;
}
