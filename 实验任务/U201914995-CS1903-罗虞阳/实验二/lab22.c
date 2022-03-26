#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>

int sem_id=0;
int countNumber=0;
int count=0;
pthread_t id1,id2,id3;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int initSemValue()//init the value of the sem
{
    union semun arg1;
    arg1.val=1;
    if(semctl(sem_id,0,SETVAL,arg1)==-1)
    {
        printf("init sem0 value fail\n");
        return 0;
    }

    union semun arg2;
    arg2.val=0;
    if(semctl(sem_id,1,SETVAL,arg2)==-1)
    {
        printf("init sem1 value fail\n");
        return 0;
    }
    union semun arg3;
    arg3.val=0;
    if(semctl(sem_id,2,SETVAL,arg3)==-1)
    {
        printf("init sem2 value fail\n");
        return 0;
    }
    return 1;
}

int delSem()
{
    union semun sem_union;
    if(semctl(sem_id,2,IPC_RMID,sem_union)==-1)
    {
        printf("del sem fail\n");
        exit(1);
    }
}

int P(int index)
{
    struct sembuf sem_b;
    sem_b.sem_num=index;//operate the 0th sem
    sem_b.sem_op=-1;
    sem_b.sem_flg=0;
    if(semop(sem_id,&sem_b,1)==-1)
    {
        printf("P fail\n");
        return 0;
    }
    return 1;
}

int V(int index)
{
    struct sembuf sem_b;
    sem_b.sem_num=index;//operate the 0th sem
    sem_b.sem_op=1;
    sem_b.sem_flg=0;
    if(semop(sem_id,&sem_b,1)==-1)
    {
        printf("V fail\n");
        return 0;
    }
    return 1;
}

void* countThread()
{
    for(int i=1;i<=100;i++)
    {
        P(0);
        countNumber+=i;
        // printf("countnumber:%d\n",countNumber);
        if(countNumber%2==1)
        {
            // printf("call up threat 1\n");
            V(1);
        }
        else
        {
            // printf("call up threat 2\n");
            V(2);
        }
    }
}

void* printThread1()
{
    while(1)
    {
        P(1);
        if(count<100)
        {
            printf("T1:%d\n",countNumber);
            count++;
        }
        else
        {
            break;
        }
        if(count==100){
            V(2);break;
        }
        V(0);
    }
}

void* printThread2()
{
    while(1)
    {
        P(2);
        if(count<100)
        {
            printf("T2:%d\n",countNumber);
            count++;
        }
        else
        {
            break;
        }
        if(count==100){
            V(1);break;
        }
        V(0);
    }
}

int main()
{
    sem_id=semget((key_t)IPC_PRIVATE,3,0666|IPC_CREAT);
    if(sem_id==-1)
    {
        printf("Creat Sem Fail\n");
        exit(1);
    }
    if(initSemValue()==0)
    {
        exit(1);
    }
    int result1=pthread_create(&id1,NULL,countThread,NULL);
    int result2=pthread_create(&id2,NULL,printThread1,NULL);
    int result3=pthread_create(&id3,NULL,printThread2,NULL);
    if(result1!=0||result2!=0||result3!=0)
    {
        printf("thread creat fail\n");
        exit(1);
    }
    printf("threat creat success\n");

    void* status[3];
    pthread_join(id1,&status[0]);
    pthread_join(id2,&status[1]);
    pthread_join(id3,&status[2]);

    delSem();
    return 0;
}
