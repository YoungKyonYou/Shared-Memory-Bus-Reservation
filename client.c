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
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define SHMKEY (key_t)60070
#define SEMKEY (key_t)60075
#define IFLAGS (IPC_CREAT|IPC_EXCL)
#define ERR ((struct bus*)-1)
#define SIZE 100

int clientId;
int semid;
struct bus{
    int id;
    int busSeat[4][11];
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
void* menu(void* arg);
void clearMemory();
void* reader(void* arg);
void reserv();
void* makeSem(void* arg);
void can();
void info();
void removeSem();
int run(int choice);
void seatInit();


void display()
{
    printf("\n\n");
    printf(ANSI_COLOR_CYAN"                      <버스 좌석표>                           \n"ANSI_COLOR_RESET);
    printf(" ");
    for(int i=0;i<58;i++)
	printf("-");
    printf("\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(b->busSeat[0][i]==0)
	{
	    printf(ANSI_COLOR_GREEN "[A%d] "ANSI_COLOR_RESET,i+1);
	   
	}
	else
	{
	    printf(ANSI_COLOR_RED "[A%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(b->busSeat[1][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[B%d] "ANSI_COLOR_RESET,i+1);
	}
	else
	{
	    printf(ANSI_COLOR_RED"[B%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(b->busSeat[2][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[C%d] "ANSI_COLOR_RESET,i+1);
	}
	else
	{
	    printf(ANSI_COLOR_RED"[C%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |");
    printf("\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(b->busSeat[3][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[D%d] "ANSI_COLOR_RESET,i+1);
	}
	else
   	    printf(ANSI_COLOR_RED"[D%d] "ANSI_COLOR_RESET,i+1);
	
    }
    printf(" |");
    printf("\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    for(int i=0;i<58;i++)
    	printf("-");
    printf("\n");
}



void seatInit()
{
    for(int i=0;i<4;i++)
    {
	for(int j=0;j<11;j++)
	{
	    b->busSeat[i][j]=0;
	}
    }
}

void removeSem(int signo)
{
     if(-1==shmctl(shmid,IPC_RMID,0))
	error_handling("shared memory delete error (client)\n");
    else
	printf("공유 메모리 제거 성공\n");
    if(-1==semctl(semid,0,IPC_RMID,0))
    	error_handling("semaphore delete error (client)\n");
    else
	printf("세마포어 커널 제거 성공\n");

}
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
void* makeMem(void* arg)
{
    if((shmid=shmget(SHMKEY,sizeof(struct bus), 0600))==-1)
    	error_handling("shared memory create error (client)\n");

    if((b=(struct bus*)shmat(shmid, 0, 0))==ERR)
	error_handling("shared memory attach error (client)\n");
    seatInit();
}
void* menu(void* arg)
{
    int* choice=arg;
    printf("---------------------------------------------\n");
    printf("      1. 버스 예약하기\n");
    printf("      2. 버스 예약 취소하기\n");
    printf("      3. 내 예약 보기\n");
    printf("      4. 종료하기\n");
    printf("---------------------------------------------\n");
    printf("\n\n입력: ");
    scanf("%d",choice);
    return NULL;
}
void clearMemory()
{
    if(-1==shmctl(shmid,IPC_RMID,0))
	error_handling("shared memory delete error (client)\n");
    else
	printf("공유 메모리 제거 성공\n");
}
void* reader(void* arg)
{
    for(;;){
	system("clear");
	display();
	printf("예약하고자 하는 좌석을  띄어쓰기로 구분해서 입력해주세요.\n");
	printf("예: A2 B6 C11\n");
	printf("입력: ");
	b->buf_nread=read(0,b->buf,SIZE);

	semop(semid, &v1,1);
	semop(semid, &p2,1);
	sleep(5);
	b->id=clientId;
	if(b->buf_nread<=0)
	{
	    printf("read error (client)\n");
	    return NULL;
	}

	b->buf_nread=read(0,b->buf,SIZE);

	semop(semid, &v1, 1);
	semop(semid, &p2, 1);

	if(b->buf_nread<=0)
	{
	    printf("read error (client)\n");
	    return NULL;
	}
    }

}

void reserv()
{
    void* thr_res;
    pthread_t t_r;
    if(pthread_create(&t_r, NULL,reader,NULL)!=0)
    	error_handling("pthread_create error (client)\n");
    if(pthread_join(t_r, thr_res)!=0)
	error_handling("pthread join() error (client)\n");

}
void can()
{
}
void info()
{

}
int run(int choice)
{
    switch(choice)
    {
	case 1: 
	    reserv();
	    break;
	case 2:
	    can();
	    break;
	case 3:
	    info();
	    break;
	case 4:
	    return -1;
	default:
	    printf("다시 입력해주세요\n");
	    break;
    }
}
void* makeSem(void* arg)
{
    semun x;
    x.val=0;

    if((semid=semget(SEMKEY,2,0600))==-1)
	error_handling("semget error (client)\n");

    if(semctl(semid,0,SETVAL,x)==-1)
	error_handling("semctl error (client)\n");

    if(semctl(semid, 1, SETVAL,x)==-1)
	error_handling("semctl error (client)\n");
    return NULL;
}
int main(int argc, char* argv[])
{
    static struct sigaction act;
    pthread_t t_id;
    void* thr_ret;
    int choice=-1,status=0;
    pid_t pid;
    clientId=atoi(argv[1]);

    act.sa_handler=removeSem;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT,&act,NULL);

    if(pthread_create(&t_id, NULL, makeMem,NULL)!=0)
    	error_handling("pthread_create makeMem  error (client)\n");
    if(pthread_join(t_id, thr_ret)!=0)
	error_handling("pthread join() makeMem error (client)\n");

    if(pthread_create(&t_id, NULL, makeSem,NULL)!=0)
    	error_handling("pthread_create makeSem error (client)\n");
    if(pthread_join(t_id, thr_ret)!=0)
	error_handling("pthread join() makeSem error (client)\n");

    while(1)
    {
   	 if(pthread_create(&t_id, NULL, menu,(void*)&choice)!=0)
		error_handling("pthread menu error (client)\n");
   	 if(pthread_join(t_id, thr_ret)!=0)
		error_handling("pthread join() error (client)\n");
	 status=run(choice);
	 if(status==-1)
	 {
	     printf("프로그램을 종료합니다\n");
	     break;
	 }
    }
    clearMemory();
    return 0;
}

