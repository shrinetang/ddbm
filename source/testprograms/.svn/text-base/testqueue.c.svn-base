#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueueNode
{
    char pBuf[256];
    int bufsize;
    int sockfd;
	struct QueueNode * pNext;
}tQueueNode;

typedef struct Queue
{
	tQueueNode *pHead;
	tQueueNode *pTail;
	int			Sum;
}tQueue;


int InitQueue(tQueue *p)
{
    if(p == NULL)
		return 1;
	p->pHead = NULL;
	p->pTail = NULL;
	p->Sum = 0;
	return 0;
}

/*
 * Delete a Queue
 */
int DeleteQueue(tQueue *p)
{
    tQueueNode * q;
    if(p == NULL)
        return 1;
    while(p->pHead != NULL)
    {
        q = p->pHead;
        p->pHead = p->pHead->pNext;
        p->Sum--;
        free((void *)q);
    }
    if(p->Sum != 0)
    {
        printf("DeleteQueue error!\n");
        return 1;
    }
    p->pHead = NULL;
	p->pTail = NULL;
	p->Sum = 0;
	return 0;
}
         
/*
 * Put in a Queue Msg to Q
 */
 
int InQueue(tQueue *p,tQueueNode * pTask)
{
    if(p == NULL || pTask == NULL)
        return 1;
    pTask->pNext = NULL;
    if(p->pHead == NULL && p->pTail == NULL) //empty queue
    {
        p->pHead = pTask;
        p->pTail = pTask;
        pTask->pNext = pTask;
        p->Sum ++;
    }
    else 
    {
        p->pTail->pNext = pTask;
        p->pTail = pTask;
        pTask->pNext = p->pHead;
        p->Sum++;
    }
    return 0;
}
           
/*
 * Get out a Queue Msg from Q
 */
tQueueNode * OutQueue(tQueue *p)
{
    tQueueNode * pTask = NULL;
    if(p->pHead == NULL || p->pTail == NULL|| p->Sum == 0)    //empty task queue
    {
        printf("a empty task queue can't out\n");
        return NULL;
    }
    if(p->pHead == p->pTail)    //only one task
    {
        pTask = p->pHead;
        pTask->pNext = NULL;
        p->pHead = NULL;
        p->pTail = NULL;
        p->Sum --;
    }
    else
    {
        pTask = p->pHead; 
        p->pHead = p->pHead->pNext;
        pTask->pNext = NULL;
        p->pTail->pNext = p->pHead;
        p->Sum--;
    }
    return pTask;
}

int main()
{
    tQueue * p = NULL;
    char cmdbuf[100];
    char tmpbuf[50];
    tQueueNode * ptask = NULL;
    tQueueNode * find = NULL;
    tQueueNode * tmptask = NULL;
    //int sockfd,bufsize;
    p = (tQueue *)malloc(sizeof(struct Queue));
    //char buf[100];
    InitQueue(p);
    //ptask = (tQueueNode *)malloc(sizeof(struct QueueNode));
    printf("please input cmd:\n");
    while(1)
    {
        fgets(cmdbuf,100,stdin);
        if(strcmp(cmdbuf,"q\n") == 0)
            break;
        ptask = (tQueueNode *)malloc(sizeof(struct QueueNode));
        sscanf(cmdbuf,"cmd is %d %s",&ptask->sockfd,tmpbuf);

        memcpy(ptask->pBuf,"\0",strlen("\0")+1);
        ptask->pNext = NULL;
        memcpy(ptask->pBuf,tmpbuf,strlen(tmpbuf)+1);
        ptask->bufsize = strlen(ptask->pBuf);

        printf("ptask you want insert:sockfd is %d,pBuf is %s,bufsize is %d\n",ptask->sockfd,\
                                                                               ptask->pBuf,\
                                                                               ptask->bufsize);
        InQueue(p,ptask);
    }
    for(find = p->pHead; ;find = find->pNext)
    {
        printf("now:sockfd is %d,pBuf is %s,bufsize is %d\n",find->sockfd,\
                                                             find->pBuf,\
                                                             find->bufsize);
        if(find == p->pTail)
            break;
    }
    while(p->pHead!= NULL)
    {
        tmptask = OutQueue(p);
        printf("outQueue:sockfd is %d,pBuf is %s,bufsize is %d\n",tmptask->sockfd,\
                                                                  tmptask->pBuf,\
                                                                  tmptask->bufsize);
        free((void *)tmptask);
    }
    system("pause");
    return 0;
}
