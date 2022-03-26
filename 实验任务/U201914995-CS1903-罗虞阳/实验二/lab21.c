#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t id1,id2;//id of the thread
int Amount=300;//the total num of the ticket
int sem_id=0;//the num of the sem set

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int initSemValue()//init the value of the sem
{
    union semun arg;
    arg.val=1;
    if(semctl(sem_id,0,SETVAL,arg)==-1)
    {
        printf("init sem value fail\n");
        return 0;
    }
    return 1;
}

int delSem()
{
    union semun sem_union;
    if(semctl(sem_id,0,IPC_RMID,sem_union)==-1)
    {
        printf("del sem fail\n");
        exit(1);
    }
}

int P()
{
    struct sembuf sem_b;
    sem_b.sem_num=0;//operate the 0th sem
    sem_b.sem_op=-1;
    sem_b.sem_flg=0;
    if(semop(sem_id,&sem_b,1)==-1)
    {
        printf("P fail\n");
        return 0;
    }
    return 1;
}

int V()
{
    struct sembuf sem_b;
    sem_b.sem_num=0;//operate the 0th sem
    sem_b.sem_op=1;
    sem_b.sem_flg=0;
    if(semop(sem_id,&sem_b,1)==-1)
    {
        printf("V fail\n");
        return 0;
    }
    return 1;
}

void * sudp1()
{
    int count=0;
    while(1)
    {
        P();
        if(Amount>0)
        {
            printf("T1: [%d] has been sold out!\n",Amount);
            count++;
            Amount--;
            V();
            usleep(3);
        }
        else
        {
            V();
            break;
        }
    }
    printf("T1 sold:%d\n",count);
}

void * sudp2()
{
    int count=0;
    while(1)
    {
        P();
        if(Amount>0)
        {
            printf("T2: [%d] has been sold out!\n",Amount);
            count++;
            Amount--;
            V();
            usleep(6);
        }
        else
        {
            V();
            break;
        }
    }
    printf("T2 sold:%d\n",count);
}

int main(void)
{
    sem_id=semget((key_t)IPC_PRIVATE,1,0666|IPC_CREAT);//CREAT A SEM
    if(sem_id==-1)
    {
        printf("Fail to Creat Sem");
        exit(0);
    }
    if(initSemValue()==0)
    {
        exit(0);
    }
    printf("sem init success\n");
    int result1=pthread_create(&id1,NULL,sudp1,NULL);
    int result2=pthread_create(&id2,NULL,sudp2,NULL);
    if(result1!=0||result2!=0)
    {
        printf("thread creat fail\n");
        exit(1);
    }

    void*status[2];
    pthread_join(id1,&status[0]);
    pthread_join(id2,&status[1]);
    delSem();
    return 0;
}
