/*ex3.c*/

#include <stddef.h>
#include "test.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define FALSE 0
#define TRUE 1


char buffer[256];
int buffer_has_item=0;
int retflag=FALSE,i=0;
pthread_mutex_t mutex;

int main()
{
	void *retval;	
	pthread_t reader;
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&reader,NULL,(void *)&readfun,NULL);
	writefun();
	pthread_join(reader,&retval);
	
}