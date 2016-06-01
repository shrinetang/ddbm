/********************************************************************/
/* Copyright (C) SSE-USTC, 2010                                     */
/*                                                                  */
/*  FILE NAME             :  taskqueue.c                            */
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
 
/*
 * Init a Queue
 */

#include "taskqueue.h"
#include <stdio.h>

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
        free(q);
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
