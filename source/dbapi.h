/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  dbapi.h                                */
/*  PRINCIPAL AUTHOR      :  Xiaolong&Tangxingyu                    */
/*  SUBSYSTEM NAME        :  dbapi                                  */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Xiaolong&Tangxingyu ,2012/11/29
 *
 */

#ifndef DBAPI_H
#define DBAPI_H 1
typedef void * tDatabase;
typedef unsigned int tKey;

typedef struct
{
    int len;/*the length of the attribute*/
    char * attribute;
}tValue;

/*
 *create a database object
 *name  :createDB
 *input :char * dbname
 *output:a pointer to TCHDB
 *in/out:
 *return:tDatabase
 */
tDatabase createDB(char *dbname);

/*
 *release the db object
 *name  : deleteDB
 *input : a tDatabase type whichi is a pointer to a object of TCHDB
 *output: a result which means if the operate is success
 *in/out:
 *return:if delete success return 0,else return -1
 */
int deleteDB(tDatabase db);

/*
 *get the value across the key,and store the result into the pointer of pvalue
 *name  : setKeyValue
 *input : a pointer to db object ,a Key type, a value type
 *output: a result which means if the operate is success
 *in/out:
 *return: if set success return 0,else return -1
 */
int setKeyValue(tDatabase db,tKey key,tValue value);

/*
 *put the key and value into the database
 *name  : getKeyValue
 *input : a pointer to db object ,a Key type, a pointer to the value type
 *output: a result which means if the operate is success
 *in/out:
 *return: if get success return 0,else return -1
 */
int getKeyValue(tDatabase db,tKey key,tValue *value);

/*
 *delete the key and value from the database
 *name  : deleteKeyValue
 *input : a pointer to db object ,a Key type, a pointer to the value type
 *output: a result which means if the operate is success
 *in/out:
 *return: if get success return 0,else return -1
 */
int deleteKeyValue(tDatabase db ,tKey key);

#endif
