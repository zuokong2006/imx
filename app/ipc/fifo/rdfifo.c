
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
	
	/* 创建fifo */
	if(mkfifo(FIFO, O_CREAT|O_EXCL) < 0)
	{
		printf("creat fifo error!\r\n");
		if(EEXIST != errno)
		{
			printf("EEXIST != errno!\r\n");
			exit(1);
		}
	}
	printf("preparing for reading bytes...\r\n");
	memset(buf, 0, sizeof(buf));
	/* 打开fifo */
	fd = open(FIFO, O_RDONLY|O_NONBLOCK, 0);
	if(-1 == fd)
	{
		printf("open fifo error!\r\n");
		exit(1);
	}
	
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		/* 读取fifo */
		if(-1 == read(fd, buf, 100))
		{
			printf("read fifo error!\r\n");
		}
		printf("read %s from %s\r\n", buf, FIFO);
		sleep(2);
	}
	
	pause();
	unlink(FIFO);
}
