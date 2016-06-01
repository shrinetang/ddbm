/********************************************************************/
/*  Copyright (C) SSE-USTC, 2012                                    */
/*                                                                  */
/*  FILE NAME             :  servercmd.c                            */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  Linux                                  */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/22                             */
/*  DESCRIPTION           :  the servercmdline control to Database  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/12/22
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "dbapi.h"
#include <pthread.h>
#define VALUESIZE 256
#define CMDSIZE 256
#define RESBUFSIZE 1024
#define MAXDBNUM 50

extern pthread_mutex_t mutex;

char dbname[100] = "\0";
//tDatabase db = NULL;
char cmdbuf[CMDSIZE];
char resbuf[RESBUFSIZE] ="\0";
/*record arrays for recording the dbaddress and create a index for return*/
typedef struct Record
{
    int dbaddr;
    //int visit;
}record;

record arrays[MAXDBNUM];

/*dbid for recording the dbname、visit count and must be according with the dbindex*/
typedef struct Dbtoid
{
    char dbname[50];
    int visit;
    int have;   //be used 1 or not 0
}dbid;

dbid store[MAXDBNUM];

/*get the rail index of cmd*/
int GetIndex(char * buf)
{
    int i,j;
    int sum = 0;
    int size = -1;
    size = strlen(buf)-1;
    if(buf[size]<48||buf[size]>57||buf[0] == 'o'||buf[0] == 'O')
        return -1;
    for(i = size ,j = 1 ; (buf[i] != ' ')&& (i > 0) ; i--)
    {
        sum += (buf[i]-48) *j;
        j = j * 10;
    }
    buf[i] = '\0';
    printf("get index is %d\n",sum);
    return sum;
}

/*from the dbindex array to find db */
int FindIndex(record * arrays,int db)
{
    int i;
    for(i = 1; (i < MAXDBNUM) && (arrays[i].dbaddr != 0); i++)
    {
        if(arrays[i].dbaddr == db )
        {
            return i;
        }
    }
    if( arrays[i].dbaddr == 0)
    {
        //printf("db before insert is %d\n",db);
        //printf("there is no this db now insert!\n");
        arrays[i].dbaddr = db;
        
        printf("db insert is %d\n",arrays[i].dbaddr);
        return i;
    }
    else //i =MAXDBNUM
    {  
            printf("DB is full!\n");
            return MAXDBNUM;
    }
    return -1;
}

int Dbnamestore(char * dbname,int index)
{
    int i;
    for(i = 1; (store[i].have !=0) && (i <MAXDBNUM); i++)
    {
        if(strcmp(store[i].dbname,dbname) == 0)
        {
            store[i].visit++;
            return i;
        }
    }
    if(index == -1)
    {
        return -1;   //表示该dbname不存在
    }
    else
    {
        if(i == MAXDBNUM)
        {
            printf("dbname store num is full!\n");
            return -1;
        }
        else    //1<i&&i<MAXDBNUM
        {
            strcpy(store[index].dbname,dbname);
            store[index].have = 1;
            store[index].visit++;
            i = index;
        }
    }
    return i;
}
        
/*check the cmdtype of CMD for to desion how to do*/
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
char * ProcessCmd(char *cmdbuf)
{
    tDatabase db = NULL;
    int index;
    int save ;
    int cmdtype = -1;
    tKey key;
    tValue setValue;
    tValue getValue;
    tValue deleteValue;
    char temps[VALUESIZE]="\0"; 
    index = GetIndex(cmdbuf);
    //printf("index is %d\n",index);
    db = (void *)arrays[index].dbaddr;
    //printf("now db is %d\n",(int)db);
    if(MatchCmd(cmdbuf,"help") == 0)
    {
        cmdtype = 0;
    }
    else if(MatchCmd(cmdbuf,"open( *|\\t).*\\.hdb") == 0)
    {
        if(db !=NULL)
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
        if(db == NULL)
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
        if(db == NULL)
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
        if(db == NULL)
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
        if(db == NULL)
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
            sscanf(cmdbuf,"open %s",dbname);
            pthread_mutex_lock(&mutex);
            save = Dbnamestore(dbname,-1);
            //printf("save is %d\n",save);
            if(save == -1)
            {               
                db = createDB(dbname);
                printf("creat db is %d\n",(int)db);
                index = FindIndex(arrays,(int)db);
                Dbnamestore(dbname,index);
            }
            else
            {
                index = save;
            }
            pthread_mutex_unlock(&mutex);       
            printf(" visit store[%d].visit = %d\n",index,store[index].visit);
            memcpy(resbuf,"db index is ",strlen("db index is ")+1);
            sprintf(resbuf+strlen("db index is "), "%d", index);
            //printf("open result size is %d\n",strlen(resbuf));
            break;
        case 2:
            sscanf(cmdbuf,"set %d %s",&key,temps);
            setValue.attribute = strstr(cmdbuf,temps);
            setValue.len = cmdbuf + VALUESIZE - setValue.attribute;
            pthread_mutex_lock(&mutex);
            if(setKeyValue(db, key, setValue) != 0)
            {
                printf("set error!\n");
                memcpy(resbuf,"set error!\n",strlen("set error!\n")+1);
                return resbuf;
            }
            pthread_mutex_unlock(&mutex);
            memcpy(resbuf,"set success!\n",strlen("set success!\n")+1);
            break;
        case 3:
            key = -1;
            getValue.attribute = temps;
            getValue.len = VALUESIZE;
            sscanf(cmdbuf,"get %d",&key);
            if(getKeyValue(db,key,&getValue) == -1)
            {
                printf("getValue error!\n");
                memcpy(resbuf,"get error!\n",strlen("get error!\n")+1);
                return resbuf;
            }
            else
            {
                printf("the key %d correspond attribute is %s\n",key,getValue.attribute);
                memcpy(resbuf,"value is ",strlen("value is ")+1);
                memcpy(resbuf+strlen("value is "),getValue.attribute,strlen(getValue.attribute)+1);
            }
            break;
        case 4:
            key = -1;
            sscanf(cmdbuf,"delete %d",&key);
            if(deleteKeyValue(db,key) == -1)
            {
                printf("deleteKeyValue error!\n");
		        memcpy(resbuf+1,"delete is OK",strlen("delete is OK")+1);
                return resbuf;
            }
            break;
        case 5:
            pthread_mutex_lock(&mutex);
            if(store[index].visit > 1)
            {
                store[index].visit --;
                printf("DB is alread closed\n");
                memcpy(resbuf,"DB is alread closed",strlen("DB is alread closed")+1);
            }
            else if(store[index].visit == 1)
                 {
		             store[index].visit --;
                     strcpy(store[index].dbname,"\0"); 
                     if(deleteDB(db) == 0)
                     {
                         arrays[index].dbaddr = 0;
                         printf("no.1 DB is alread closed\n");
                         memcpy(resbuf,"no.1 DB is alread closed",strlen("no.1 DB is alread closed")+1);
                     }
                 }
                 else
                 {
                    printf("Close DB error!\n");
                    memcpy(resbuf+1,"Close DB error!\n",strlen("Close DB error!\n")+1);
                 }
            pthread_mutex_unlock(&mutex);
            break;
        default:
            printf("command is wrong\n");
            memcpy(resbuf+0,"command wrong!\n",strlen("command wrong!\n")+1);
            break;
    }
    return resbuf;
}



