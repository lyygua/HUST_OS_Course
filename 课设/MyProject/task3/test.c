#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_SIZE 1024
 
int main(void)
{
	int fd;
	char buf[MAX_SIZE];	//������
	char get[MAX_SIZE];	//Ҫд�����Ϣ
 
	char dir[50] = "/dev/lgsDevice";
 
	fd = open(dir, O_RDWR | O_NONBLOCK);//���豸
 
	if (fd != -1)
	{
		//����ʼ��Ϣ
		read(fd, buf, sizeof(buf));
		printf("%s\n", buf);
 
		//д��Ϣ
		printf("input :");
		gets(get);
		write(fd, get, sizeof(get));
 
		//���ղ�д����Ϣ
		read(fd, buf, sizeof(buf)); 
		printf("device Message: %s\n", buf);
		
		close(fd);
		return 0;
	}
	else
	{
		printf("Device open failed\n");
		return -1;
	}
}
