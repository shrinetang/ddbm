/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  remotedbapi.h                          */
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

#ifndef DBAPI_H
#define DBAPI_H   1
typedef void * tDatabase;
typedef unsigned int tKey;

typedef struct
{
    int len;
    char * attribute;
}tValue;

/*
 *send createDB cmd and recv the op result
 *name  :createDB
 *input :cmdbuf
 *output:send info and op result
 *in/out:
 *return:index of db
 */
int createDB(char * buf,char * Ip_Addr,int Port,int * sock);

/*
 *send deleteDB cmd and recv the op result
 *name  :deleteDB
 *input :index of db
 *output:send info and op result
 *in/out:
 *return:success 0/fail exit
 */
int deleteDB(int dbindex,int sockfd);
/*
 *send setKeyValue cmd and recv the op result
 *name  :setKeyValue
 *input :cmdbuf and dbindex
 *output:send info and op result
 *in/out:
 *return:success 0/fail exit
 */
int setKeyValue(char * buf,int dbindex,int sockfd);

/*
 *send getKeyValue cmd and recv the op result
 *name  :getKeyValue
 *input :cmdbuf and dbindex
 *output:send info and op result
 *in/out:
 *return:success 0/fail exit
 */
int getKeyValue(char * buf,int dbindex,int sockfd);

/*
 *send deleteKeyValue cmd and recv the op result
 *name  :deleteKeyValue
 *input :cmdbuf and dbindex
 *output:send info and op result
 *in/out:
 *return:success 0/fail exit
 */
int deleteKeyValue(char * buf,int dbindex,int sockfd);

#endif
