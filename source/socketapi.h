/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  socketapi.h                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  socketapi                              */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/26                             */
/*  DESCRIPTION           :  the client socket                      */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/26
 *
 */
 
#ifndef SOCKETAPI_H
#define SOCKETAPI_H   1

/*
 * Initclient
 * input	: ip and port
 * output	: none
 * in/out	: none
 * return	: if sucess return sockfd
 */
int Initclient(char * Ip_Addr,int Port);

/*
 * OpenRemoteService
 * input	: none
 * output	: none
 * in/out	: none
 * return	: if sucess return 0
 */
int OpenRemoteService();

/*
 * CloseRemoteService
 * input	: sockfd 
 * output	: none
 * in/out	: none
 * return	: if sucess return 0
 */
int CloseRemoteService(int sockfd);

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

#endif 
