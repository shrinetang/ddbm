#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define FALSE 0
#define TRUE 1


char buffer[256];
int buffer_has_item=0;
int retflag=FALSE,i=0;


void readfun()
{
	while(1)
	{
		if(retflag)
			return;
		pthread_mutex_lock(&mutex);
		if(buffer_has_item==1)
		{
			printf("%s",buffer);
			buffer_has_item=0;
		}
		pthread_mutex_unlock(&mutex);
	}
}
void writefun()
{
	int i=0;	
	while(1)
	{
		if(i==10)
		{
			retflag=TRUE;
			return;
		}		
		pthread_mutex_lock(&mutex);
		if(buffer_has_item==0)
		{
			sprintf(buffer,"This is %d\n",i++);
			buffer_has_item=1;
		}
		pthread_mutex_unlock(&mutex);	
	}
}



struct sem_id
{
    struct ipc_perm sem_perm;   //指向相对应的ipc_perm结构指针
    struct sem  * sem_base;     //信号量数组首地址指针
    ushort sem_nsems;           //信号量个数
    time_t sem_otime;           //最近一次调用semop时间
    time_t sem_ctime;           //最近一次改变时间
};

struct sem
{
    ushort semval;  //信号量的值
    pid_t  sempid;  //最后一次操作信号量的进程的进程号
    ushort semncnt; //等待申请资源进程数（阻塞数）
    ushort semzcnt; //等待全部资源可被独占进程数
};

#include <sys/type.h>
#include <sys/ipc.h>
#include <sys/sem.h>



/*function 创建信号量集
 *input  : 关键字ket(ipc信号量标示符）nsems 信号量集中应该创建信号量的数目 flag \
           flag 取0 或者 IPC_NOWAIT 不等待直接退出报错
 *output : 
 *in/out :
 *return : success 信号量集合标识符 /failure -1
 */
int semget(key_t key, int nsems, int flag);    
//该函数为创建信号量集的函数 

/*function 该函数相当于pv操作中的v操作，释放回收资源所用
 *input  :
 *output :
 *in/out :
 *return :
 */
int semop(int semid, struct sembuf * sops, size_t nops)

struct sembuf
{
    ushort sem_num; //待处理的信号量在信号集中的序号
    short sem_op;   //该值表示要执行的操作
    short sem_flg;  //操作标记
}

sem_op代表的含义：
sem_op>0 进程使用完资源交回
sem_op<0 进程申请资源
         当信号量的值>=|sem_op| 表示有足够的资源可以分给该进程，将信号的值中减去|sem_op|,表示分配给进程那么多资源
         当信号量的值< |sem_op| 表示没有足够的资源分配，
             如果sem_flag中的IPC_NOWAIT位被置1，函数立刻带错返回
             如果sem_flag中的IPC_NOWAIT位未被置1，则与这个信号量相关的sem结构中的semncnt(等待进程数)加1，该进程
                进入阻塞状态，直到其他进程释放了资源且足够分配，该进程才被唤醒
         当信号量的值 == 0 函数立刻返回
         当信号量的值不为0，与这个信号量相关的sem结构中的semzcnt值加一，进程阻塞，直到信号量的值变为0才被唤醒，信
                号量被删除则带错返回

/*function 
 *input  :
 *output :
 *in/out :
 *return :
 */
 int semctl(int semid, int semnum, int cmd, [union semun arg]);
 参数说明：
 semid : 信号量集合标识
 semnum: 信号量集合中的一个信号量编号
 cmd :   对信号量操作的命令
 arg :   操作命令提供的参数
 该函数可以用来初始化信号量值，管理信号量访问权限，获取与信号量相关的结构体的值，查看信号量相关的进程数等信息。 
 
 
 /*function 
 *input  :
 *output :
 *in/out :
 *return :
 */