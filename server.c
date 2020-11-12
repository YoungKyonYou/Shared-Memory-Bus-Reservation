#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/sem.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#define SHMKEY (key_t)60070
#define SEMKEY (key_t)60075
#define IFLAGS (IPC_CREAT|IPC_EXCL)
#define ERR ((struct bus*)-1)
#define SIZE 100

int semid;
struct bus{
    int id;
    int butSeat[4][11];
    char buf[SIZE];
    int buf_nread;
};
struct bus* b;
static int shmid;

struct sembuf p1={0,-1,0}, p2={1,-1,0};
struct sembuf v1={0,1,0}, v2={1,1,0};

typedef union _semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
}semun;
void error_handling(char* message);
void* makeMem(void* arg);
void clearMemory();
void* makeSem(void* arg);
void* writer(void* arg);
void* writer(void* arg)
{
    printf("server inside writer function()\n");
    for(;;){
	semop(semid, &p1, 1);
	semop(semid, &v2,1);
	if(b->buf_nread<=0)
	{
	    printf("write error (server)\n");
	    return NULL;
	}

	//write(1,b->buf,b->buf_nread);
	printf("출력1:%s\n",b->buf);
	/*char *ptr = strtok(b->buf, " ");     
  	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    	{
    	    printf("%s\n", ptr);          // 자른 문자열 출력
       	    ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
  	}*/
	semop(semid, &p1, 1);
	semop(semid, &v2,1);

	if(b->buf_nread<=0)
	{
	    printf("write error (server)\n");
	    return NULL;
	}
	
	printf("출력2:%s\n",b->buf);
	//write(1,b->buf,b->buf_nread);
    }
}












void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
void* makeMem(void* arg)
{
    if((shmid=shmget(SHMKEY,sizeof(struct bus), 0600|IFLAGS))==-1)
    	error_handling("shared memory create error (server)\n");
    if((b=(struct bus*)shmat(shmid, 0, 0))==ERR)
	error_handling("shared memory attach error (server)\n");
}

void clearMemory()
{
    if(-1==shmctl(shmid,IPC_RMID,0))
	error_handling("shared memory delete error (server)\n");
    else
	printf("공유 메모리 제거 성공\n");
}
void* makeSem(void* arg)
{
    semun x;
    x.val=0;

    if((semid=semget(SEMKEY,2,0600|IFLAGS))==-1)
	error_handling("semget error (server)\n");

    if(semctl(semid,0,SETVAL,x)==-1)
	error_handling("semctl error (server)\n");

    if(semctl(semid, 1, SETVAL,x)==-1)
	error_handling("semctl error (server)\n");
    return NULL;
}
int main(void)
{
    pthread_t t_id;
    void* thr_ret;
    int choice=-1,status=0;;
    pid_t pid;
    if(pthread_create(&t_id, NULL, makeMem,NULL)!=0)
    	error_handling("pthread_create makeMem  error (server)\n");
    if(pthread_join(t_id, thr_ret)!=0)
	error_handling("pthread join() makeMem error (server)\n");


    if(pthread_create(&t_id, NULL, makeSem,NULL)!=0)
    	error_handling("pthread_create makeSem error (server)\n");
    if(pthread_join(t_id, thr_ret)!=0)
	error_handling("pthread join() makeSem error (server)\n");

    while(1)
    {
   	 if(pthread_create(&t_id, NULL, writer,NULL)!=0)
		error_handling("pthread menu error (server)\n");
   	 if(pthread_join(t_id, thr_ret)!=0)
		error_handling("pthread join() error (server)\n");
    }
    clearMemory();
    return 0;
}

