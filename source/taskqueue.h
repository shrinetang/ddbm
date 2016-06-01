/********************************************************************/
/* Copyright (C) SSE-USTC, 2010                                     */
/*                                                                  */
/*  FILE NAME             :  taskqueue.h                            */
/*  PRINCIPAL AUTHOR      :  tangxingyu                             */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  taskqueue                              */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2010/12/28                             */
/*  DESCRIPTION           :  interface of msg Queue	                */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by tangxingyu,2010/12/28
 *
 */

#ifndef TASKQUEUE_H
#define TASKQUEUE_H

/*
 * Queue DataNode Type
 */
typedef struct QueueNode
{
    char pBuf[256]; //Ip_Addr
    int bufsize;    //Port
    int sockfd;     //sockfd
    int dbindex;
    struct QueueNode * pNext;
}tQueueNode;

/*
 * Queue Node Type
 */
typedef struct Queue
{
	tQueueNode *pHead;
	tQueueNode *pTail;
	int Sum;
}tQueue;

/*
 * Create a Queue
 */
int InitQueue(tQueue *p);

/*
 * Delete a Queue
 */
int DeleteQueue(tQueue *p);

/*
 * Put in a Queue Msg to Q
 */
 
int InQueue(tQueue *p,tQueueNode * pTask);

/*
 * Get out a Queue Msg from Q
 */
tQueueNode * OutQueue(tQueue *p);

#endif 


