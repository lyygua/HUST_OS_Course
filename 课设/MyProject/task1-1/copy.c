#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define bufsize 4

int main(int argc, const char* argv[])
{
	int fread = open(argv[1], O_RDONLY);//��ֻ����ʽ�򿪶�ȡ�ļ� 
	if(fread == -1)
	{
		printf("error: failed to open %s\n", argv[1]);
		return -1;
	}
	// O_RDWR:�ļ��ɶ�д 
	// O_CREAT:�����ļ��������򴴽���
	// O_TRUNC:�����ҿ�дʱ�����ԭ����Ϣ
	int fwrite = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);//��д�ļ� 
	if(fwrite == -1)
	{
		printf("error: failed to open %s\n", argv[2]);
		close(fread);
		return -1;
	}

	char buf[bufsize];
	int num = 0;

	while((num = read(fread, buf, bufsize)) > 0)//�����ļ��е����ݷ���buf����,Ȼ��buf�����ݷ���д�ļ� 
		write(fwrite, buf, num);
	printf("ok\n");

	close(fread);
	close(fwrite);

	return 0;
}
