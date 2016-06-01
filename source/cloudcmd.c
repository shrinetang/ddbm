/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  clientcmd                              */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  client                                 */
/*  MODULE NAME           :  clientcmd                              */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/12/29                             */
/*  DESCRIPTION           :  database application client cmdinput   */
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
#include <regex.h>
#include "clouddbapi.h"
#define CMD_NUM 6
#define DBNAMESIZE 1024
#define CMDSIZE 128
#define VALUESIZE 256

char cmdbuf[CMDSIZE] = "\0";;
const char *prompt = "MyDB";
char dbname[DBNAMESIZE] = "\0";

int GetCmd(char * cmdbuf,int size);
int ProcessCmd(char * cmdbuf);
int MatchCmd(char * cmdbuf,char * mode);
int dbindex = -1;

int main(void)
{
    while(1)
    {
        printf("%s::%s>>",prompt,dbname);
        GetCmd(cmdbuf,CMDSIZE);
        if(MatchCmd(cmdbuf , "q" ) == 0 || MatchCmd(cmdbuf ,"Q" ) == 0)
        {
            break; 
        }
        else
        {
            ProcessCmd(cmdbuf);
        }
    }
    return 0;
}

/*get the command*/
int GetCmd(char * cmdbuf,int size)
{
    fgets(cmdbuf, CMDSIZE, stdin);
    return 0;
}

/*check the command*/
int MatchCmd(char * cmdbuf,char * mode)
{
    regex_t reg;
    regmatch_t pm[1];
    const size_t nmatch = 1;
    char ebuf[128];
    regcomp(&reg,mode,REG_EXTENDED | REG_ICASE | REG_NEWLINE);
    if(regexec(&reg,cmdbuf,nmatch,pm,0) == 0)
    {
	    regfree(&reg);
	    return 0;
    }	
    regfree(&reg);
    return 1;
}

/*complish the command*/
int ProcessCmd(char *cmdbuf)
{
    int cmdtype = -1;
    int cmdsize = -1;
    cmdsize = strlen(cmdbuf);
    if(MatchCmd(cmdbuf,"help") == 0)
    {
        cmdtype = 0;
    }
    else if(MatchCmd(cmdbuf,"open( *|\\t).*\\.hdb") == 0)
    {
        if(dbindex != -1)
        {
            printf("please close a database first.\n");
            return 0;
        }
        else
        {
            cmdtype = 1;
        }
    }
    else if(MatchCmd(cmdbuf,"set [0-9]* .*") == 0)
    {
        if(dbindex == -1)
        {
            printf("please open a database.\n");
            return 0;
        }
        else
        {
            cmdtype = 2;
        }
    }
    else if(MatchCmd(cmdbuf,"get [0-9]") == 0)
    {
        if(dbindex == -1)
        {
            printf("please open a database.\n");
            return 0;
        }
        else
        {
            cmdtype = 3;
        }
    }
    else if(MatchCmd(cmdbuf,"delete [0-9]") == 0)
    {
        if(dbindex == -1)
        {
            printf("please open a database.\n");
            return 0;
        }
        else
        {
            cmdtype = 4;
        }
    }
    else if(MatchCmd(cmdbuf,"close") == 0)
    {
        if(dbindex == -1)
        {
            printf("please open a database.\n");
            return 0;
        }
        else
        {
            cmdtype = 5;
        }
    }

    switch(cmdtype)
    {
        case 0:
            printf("help              --require help\n");
            printf("open file         --eg:open dbtest.hdb\n");
            printf("set key valude    --eg:set 100 helloword\n");
            printf("get key           --eg:get 100\n");
            printf("delete key        --eg:delete 100\n");
            printf("close             --close the database\n");
            printf("q/Q               --exit this commandline\n");
            break;
        case 1:
            dbindex = CloudDbcreate(cmdbuf);
            break;
        case 2:
            cmdbuf[cmdsize] = ' ';
            cmdbuf[cmdsize+1] = '\0';
            if(CloudDbsetKeyValue(cmdbuf)!= 0)
            {
                printf("set error!\n");
                return 1;
            }
            break;
        case 3:
            cmdbuf[cmdsize] = ' ';
            cmdbuf[cmdsize+1] = '\0';
            if(CloudDbgetKeyValue(cmdbuf) != 0)
            {
                printf("getValue error!\n");
                return 1;
            }
            break;
        case 4:
            cmdbuf[cmdsize] = ' ';
            cmdbuf[cmdsize+1] = '\0';
            if(CloudDbdeleteKeyValue(cmdbuf) != 0)
            {
                printf("deleteKeyValue error!\n");
                return 1;
            }
            break;
        case 5:
            cmdbuf[cmdsize] = ' ';
            cmdbuf[cmdsize+1] = '\0';
            if(CloudDbclose() == 0)
            {
                //printf("DB is alread closed\n");
            }
            else
            {
               printf("Close DB error!\n");
            }
            dbindex = -1;
            break;
        default:
            printf("command is wrong\n");
            break;
    }
    return 0;
}
