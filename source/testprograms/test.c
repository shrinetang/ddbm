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
    struct ipc_perm sem_perm;   //ָ�����Ӧ��ipc_perm�ṹָ��
    struct sem  * sem_base;     //�ź��������׵�ַָ��
    ushort sem_nsems;           //�ź�������
    time_t sem_otime;           //���һ�ε���semopʱ��
    time_t sem_ctime;           //���һ�θı�ʱ��
};

struct sem
{
    ushort semval;  //�ź�����ֵ
    pid_t  sempid;  //���һ�β����ź����Ľ��̵Ľ��̺�
    ushort semncnt; //�ȴ�������Դ����������������
    ushort semzcnt; //�ȴ�ȫ����Դ�ɱ���ռ������
};

#include <sys/type.h>
#include <sys/ipc.h>
#include <sys/sem.h>



/*function �����ź�����
 *input  : �ؼ���ket(ipc�ź�����ʾ����nsems �ź�������Ӧ�ô����ź�������Ŀ flag \
           flag ȡ0 ���� IPC_NOWAIT ���ȴ�ֱ���˳�����
 *output : 
 *in/out :
 *return : success �ź������ϱ�ʶ�� /failure -1
 */
int semget(key_t key, int nsems, int flag);    
//�ú���Ϊ�����ź������ĺ��� 

/*function �ú����൱��pv�����е�v�������ͷŻ�����Դ����
 *input  :
 *output :
 *in/out :
 *return :
 */
int semop(int semid, struct sembuf * sops, size_t nops)

struct sembuf
{
    ushort sem_num; //��������ź������źż��е����
    short sem_op;   //��ֵ��ʾҪִ�еĲ���
    short sem_flg;  //�������
}

sem_op����ĺ��壺
sem_op>0 ����ʹ������Դ����
sem_op<0 ����������Դ
         ���ź�����ֵ>=|sem_op| ��ʾ���㹻����Դ���Էָ��ý��̣����źŵ�ֵ�м�ȥ|sem_op|,��ʾ�����������ô����Դ
         ���ź�����ֵ< |sem_op| ��ʾû���㹻����Դ���䣬
             ���sem_flag�е�IPC_NOWAITλ����1���������̴�����
             ���sem_flag�е�IPC_NOWAITλδ����1����������ź�����ص�sem�ṹ�е�semncnt(�ȴ�������)��1���ý���
                ��������״̬��ֱ�����������ͷ�����Դ���㹻���䣬�ý��̲ű�����
         ���ź�����ֵ == 0 �������̷���
         ���ź�����ֵ��Ϊ0��������ź�����ص�sem�ṹ�е�semzcntֵ��һ������������ֱ���ź�����ֵ��Ϊ0�ű����ѣ���
                ������ɾ���������

/*function 
 *input  :
 *output :
 *in/out :
 *return :
 */
 int semctl(int semid, int semnum, int cmd, [union semun arg]);
 ����˵����
 semid : �ź������ϱ�ʶ
 semnum: �ź��������е�һ���ź������
 cmd :   ���ź�������������
 arg :   ���������ṩ�Ĳ���
 �ú�������������ʼ���ź���ֵ�������ź�������Ȩ�ޣ���ȡ���ź�����صĽṹ���ֵ���鿴�ź�����صĽ���������Ϣ�� 
 
 
 /*function 
 *input  :
 *output :
 *in/out :
 *return :
 */