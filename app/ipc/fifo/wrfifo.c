
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>  
#include <fcntl.h>

/* FIFO dir */
#define FIFO		"/tmp/myfifio"

/* main function */
int main(int argc, char **argv) 
{
	char buf[100];
	int fd;
	
	/* 输入参数检查 */
	if(1 == argc)
	{
		printf("please send something!\r\n");
		exit(1);
	}
	strcpy(buf, argv[1]);
	/* 打开fifo */
	fd = open(FIFO, O_WRONLY|O_NONBLOCK, 0);
	if(-1 == fd)
	{
		printf("open fifo error!\r\n");
		exit(1);
	}
	/* 写fifo */
	if(-1 == write(fd, buf, 100))
	{
		printf("write fifo error!\r\n");
		exit(1);
	}
	else
	{
		printf("write %s to the %s", buf, FIFO);
	}
}
