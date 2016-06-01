/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  servernew.c                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server socket                          */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/26                             */
/*  DESCRIPTION           :  the serversocket                       */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/26
 *
 */
 
#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H    1
/*
 * ServerPrepare
 * input : ServerId
 * output: none
 * in/out: none
 * return: none    
 */
int ServerPrepare(char * ServerId);

/*
 * ServerPrepare
 * input : ip,port
 * output: none
 * in/out: none
 * return: if recv the msg of new_fd which is all ready listened by epoll , return the msg    
 */
int ServerOpen();

/*
 * SendData
 * input	: sockfd ,sendmsg
 * output	: none
 * in/out	: none
 * return	: if sucess return BufSize sended
 *          : if failure return (-1)
 */
int SendData(int sockfd, char * sendmsg);

/*
 * RecvData
 * input	: sockfd
 * output	: none
 * in/out	: none
 * return	: if sucess return result
 */
char * RecvData(int sockfd);

/*
 * CloseServer
 * input	: none
 * output	: none
 * in/out	: none
 * return	: if success return 0
 */
void CloseServer();

#endif
