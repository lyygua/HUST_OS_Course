#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<wait.h>
#define MAX 1024
int count1=0;
int count2=0;
int pid1=0;
int pid2=0;
int pipe_field[2];//0:wirte 1:read
char string[MAX];
const int max=10;

void handle(int seqnum)
{
    kill(pid1,SIGUSR1);
    kill(pid2,SIGUSR2);
    close(pipe_field[1]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
    printf("Parent Process is Killed!\n");
}

void handle1(int seqnum)
{
    close(pipe_field[0]);
    printf("the number of the received message:%d\n",count1);
    printf("Child Process 1 is Killed by Parent!\n");
    exit(0);

}

void handle2(int seqnum)
{
    close(pipe_field[0]);
    printf("the number of the received message:%d\n",count2);
    printf("Child Process 2 is Killed by Parent!\n");
    exit(0);

}


int main(void)
{
    if(pipe(pipe_field)<0){
        printf("pipe creat fail!\n");
        return -1;
    }
    else//pipe creat success
    {
        printf("pipe creat success\n");
        signal(SIGINT,handle);
        
        pid1=fork();
        if(pid1==0)
        {
            signal(SIGUSR1,handle1);
            signal(SIGINT,SIG_IGN);
            close(pipe_field[1]);
            while(1)
            {
                read(pipe_field[0],string,MAX);
                count1++;
                printf("p1:%s",string);
            }

        }
        else
        {
            pid2=fork();
            if(pid2==0)
            {
                signal(SIGINT,SIG_IGN);
                signal(SIGUSR2,handle2);
                close(pipe_field[1]);
                while(1)
                {
                    read(pipe_field[0],string,MAX);
                    count2++;
                    printf("p2:%s",string);
                }
            }
        }
        int count=1;
        close(pipe_field[0]);
        while(1)
        {
            sprintf(string,"I send you %d times\n",count++);
            write(pipe_field[1],string,MAX);
            if(count>=max) break;
            sleep(1);
        }
        kill(pid1,SIGUSR1);
        kill(pid2,SIGUSR2);
        waitpid(pid1,NULL,0);
        waitpid(pid2,NULL,0);
        close(pipe_field[1]);
        printf("Parent Process is Killed!\n");
        return 0;
    }
    
}
