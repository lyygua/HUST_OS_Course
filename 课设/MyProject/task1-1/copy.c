#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define bufsize 4

int main(int argc, const char* argv[])
{
	int fread = open(argv[1], O_RDONLY);//以只读方式打开读取文件 
	if(fread == -1)
	{
		printf("error: failed to open %s\n", argv[1]);
		return -1;
	}
	// O_RDWR:文件可读写 
	// O_CREAT:若此文件不存在则创建它
	// O_TRUNC:存在且可写时会清除原来信息
	int fwrite = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);//打开写文件 
	if(fwrite == -1)
	{
		printf("error: failed to open %s\n", argv[2]);
		close(fread);
		return -1;
	}

	char buf[bufsize];
	int num = 0;

	while((num = read(fread, buf, bufsize)) > 0)//将读文件中的内容放入buf缓冲,然后将buf中内容放入写文件 
		write(fwrite, buf, num);
	printf("ok\n");

	close(fread);
	close(fwrite);

	return 0;
}
