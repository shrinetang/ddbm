/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  servercmd.h                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  servercmd                              */
/*  LANGUAGE              :  Any                                    */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/22                             */
/*  DESCRIPTION           :  the servercmd                          */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/22
 *
 */

#ifndef SERVERCMD_H
#define SERVERCMD_H

/*
 *matchcmd for finding which cmd should be executing
 *name  :MatchCmd
 *input :char * cmdbuf,char * mode
 *output:
 *in/out:
 *return:match success 0/match fail 1
 */
int MatchCmd(char * cmdbuf,char * mode);

/*
 *processcmd and return the result of processing
 *name  :ProcessCmd
 *input :cmdbuf
 *output:the infomation of processing 
 *in/out:
 *return:the result of processing
 */
char * ProcessCmd(char *cmdbuf);

#endif

