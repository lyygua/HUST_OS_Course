#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
int main()
{

    long int aaa=syscall(333,"test.txt","222.txt");//ʹ��333�ŵ��� 
    printf("%ld\n",aaa);
    return 0;
}
