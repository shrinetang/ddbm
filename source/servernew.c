/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  servernew.c                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server socket                          */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/22                             */
/*  DESCRIPTION           :  the serversocket                       */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/22
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
#include "serversocket.h"
#include "servercmd.h"
#include "taskqueue.h"
#include <semaphore.h>
#define MAX_THREADNUM 3

#ifdef _DEBUG
    #define debug_print(s)  printf s
#else
    #define debug_print(s)
#endif

pthread_mutex_t mutex;
pthread_t thread_id[MAX_THREADNUM];
sem_t pSems[MAX_THREADNUM];

int AllocateTask(int thread_no);
int ServerProcess(int new_fd, char * recvbuf);

tQueue * p = NULL;
tQueueNode * ptask;

/*server engine start function */
int main(int argc,char *argv[])
{
    char * Ip_Addr = NULL;
    char * sPort;
    char * tmpptr;
    char * ServerId = NULL;
    int Port;
    ServerId = argv[1];
    int new_fd,i,j,thread_no;
    debug_print(("Server Engine Open!\n"));
    /*��ʼ���������*/
    p = (tQueue *)malloc(sizeof(struct Queue));
    InitQueue(p);
    /*����MAX_THREADNUM���̣߳�����ʼ�����̶߳�Ӧ���ź���*/
    for(i = 0;i < MAX_THREADNUM ; i++)
    {
        thread_no = i;
        sem_init(&pSems[i],0,0); //��ʼ���߳��ź���
        if(pthread_create(&thread_id[i] ,NULL,(void*)AllocateTask,(void*)thread_no) != 0)
        {
            fprintf(stderr,"pthread_create Error,%s:%d\n",__FILE__,__LINE__);
            exit(-1);
        }
    }         
    ServerPrepare(ServerId);    
    printf("server engine open!\n");
    pthread_mutex_init(&mutex, NULL);
    while(1)
    {
        new_fd = ServerOpen();
        ptask = (tQueueNode *)malloc(sizeof(struct QueueNode));
        ptask->sockfd = new_fd;
        tmpptr = RecvData(new_fd);
        memcpy(ptask->pBuf,tmpptr,strlen(tmpptr)+1);
        ptask->bufsize = strlen(ptask->pBuf);
        debug_print(("ptask you want insert:sockfd is %d,pBuf is %s,bufsize is %d\n",ptask->sockfd,\
                                                                               ptask->pBuf,\
                                                                               ptask->bufsize));
        InQueue(p,ptask);   //����յ���������ӵȺ���
        j = (rand()%3);     //����������е�һ���߳̽���Դ��������߳�
        debug_print(("V op +%d thread\n",j));
        sem_post(&pSems[j]);   //���ź���v��������Դ��Ŀ+1
    }
    ServerCloseinfo(ServerId);
    CloseServer();
    for(i = 0;i < MAX_THREADNUM;i++)
    {
        sem_destroy(&pSems[i]);
        DeleteQueue(p);
    } 
    return 0;
}

int AllocateTask(int thread_no)
{
    while(1)
    {
        sem_wait(&pSems[thread_no]); 
        debug_print(("allocating a event!\n"));
        pthread_mutex_lock(&mutex);
        tQueueNode * ptask = NULL;
        ptask = OutQueue(p);
        pthread_mutex_unlock(&mutex);
        ServerProcess(ptask->sockfd,ptask->pBuf);
        debug_print(("ServerProcess finish\n"));
        free(ptask);
    }
}
            
int ServerProcess(int new_fd, char * recvbuf)
{
    char * result = NULL;
    //strcpy( sendbuf, "welcome !");
    //sendlen = strlen(sendbuf);
    //send(new_fd, sendbuf, sendlen,0); 
    result = ProcessCmd(recvbuf);
    debug_print(("result is %s\n",result));
    SendData(new_fd,result);
    return 0;
}        
        
        
        
        
