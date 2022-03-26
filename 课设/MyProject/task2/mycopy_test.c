#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
int main()
{

    long int aaa=syscall(333,"test.txt","222.txt");//使用333号调用 
    printf("%ld\n",aaa);
    return 0;
}
