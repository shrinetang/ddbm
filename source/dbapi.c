/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  dbapi.c                                */
/*  PRINCIPAL AUTHOR      :  Xiaolong &Tangxingyu                   */
/*  SUBSYSTEM NAME        :  dbapi                                  */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  LINUX                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  database application interface (dbapi) */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu,2012/11/29
 *
 */

#include "dbapi.h"
#include <tcutil.h>
#include <tchdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
/*
int dbint[100];
int count=0;
 */
/*create a database object*/
tDatabase createDB(char *dbname)
{
    int ecode;
    TCHDB *db;
    int i;
    db = tchdbnew();
    tchdbsetmutex(db);
    /* open the database */
    if(!tchdbopen(db, dbname, HDBOWRITER | HDBOCREAT))
    {
        ecode = tchdbecode(db);
	fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
	exit(-1);
    }
    return (tDatabase)db;
	
}

/*release the db object*/
int deleteDB(tDatabase db)
{
    int ecode;
    if(!tchdbclose(db))
    {
	ecode = tchdbecode(db);
	fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
        return -1;
    }
    /* delete the object */
    tchdbdel(db);
    return 0;
}

/*put the key and value into the database*/
int setKeyValue(tDatabase db,tKey key,tValue value)
{
    int ecode;
    if(!tchdbput(db, &key,sizeof(tKey),value.attribute,value.len ))
    {
        ecode = tchdbecode(db);
	fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));
	return -1;
    }
    return 0;
}

/*get the value across the key,and store the result into the pointer of pvalue*/
int getKeyValue(tDatabase db,tKey key,tValue *pvalue)
{
    int ecode;
    int ret;
    ret=tchdbget3(db,&key,sizeof(tKey),pvalue->attribute,pvalue->len);
    if(ret)
    {
        pvalue->attribute[ret]='\n';
        pvalue->len=ret;	
	    return 0;
    }
    ecode=tchdbecode(db);
    fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));	
    return -1;
}

/*delete a record from the database*/
int deleteKeyValue(tDatabase db ,tKey key)
{ 
    if(tchdbout(db, &key,sizeof(tKey)))
        return 1;
    else 
        return 0;  
}
