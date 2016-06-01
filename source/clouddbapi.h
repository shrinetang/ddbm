/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  remotedbapi.c                          */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  remotedbapi                            */
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
#ifndef CLOUDDBAPI_H
#define CLOUDDBAPI_H  1
/*
 *connect with all server and create db
 *name  :CloudDbcreate
 *input :char * buf
 *output:the result of open
 *in/out:
 *return:db index
 */
int CloudDbcreate(char * buf);
 
/*
 *set a key-value to DB
 *name  :CloudDbsetKeyValue
 *input :char * buf
 *output:the result of set
 *in/out:
 *return:success 0
 */
int CloudDbsetKeyValue(char * buf);

/*
 *get the value of the key
 *name  :CloudDbgetKeyValue
 *input :char * buf
 *output:the result of get
 *in/out:
 *return:success 0
 */
int CloudDbgetKeyValue(char * buf);

/*
 *delete the key
 *name  :CloudDbdeleteKeyValue
 *input :char * buf
 *output:the result of delete
 *in/out:
 *return:success 0
 */ 
int CloudDbdeleteKeyValue(char * buf);

/*
 *close all the connection
 *name  :CloudDbclose
 *input :
 *output:the result of close
 *in/out:
 *return:success 0
 */ 
int CloudDbclose();
 
#endif