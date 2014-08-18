
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h> 

#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"
#include "deftype.h"

int32 main(int32 argc, int8 *argv[]) 
{
	int32 fd = 0, len = 0;
	int8 *homedir, *filedir;
	int8 buf[1024] = {0};
	
	/* 输入参数判断 */
	if(2 != argc)
	{
		DEBUG_MSG("usage: ./file_test string\r\n");
		exit(1);
	}
	
	DEBUG_MSG("main function\r\n");
	
	/* 获取HOME路径 */
	homedir = getenv("HOME");
	if(NULL == homedir)
	{
		DEBUG_MSG("getenv() error\r\n");
		exit(1);
	}
	filedir = malloc(strlen(homedir)+strlen("/file_test_temp.txt")+2);
	if(NULL == filedir)
	{
		DEBUG_MSG("malloc() error\r\n");
		exit(1);
	}
	strcpy(filedir, homedir);
	strcat(filedir, "/file_test_temp.txt");
	DEBUG_MSG("file dir:%s\r\n", filedir);
	
	/* 打开文件 */
	fd = open(filedir, O_CREAT|O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
	if(0 > fd)
	{
		free(filedir);
		DEBUG_MSG("open error\r\n");
		exit(1);
	}
	/* 写文件 */
	len = write(fd, argv[1], strlen(argv[1]));
	if(len < strlen(argv[1]))
	{
		DEBUG_MSG("write file len < strlen(argv[1])!\r\n");
	}
	else
	{
		DEBUG_MSG("write file ok!\r\n");
	}
	/* 关闭文件 */
	free(filedir);
	close(fd);
	
	
	while(1)
	{
		fd = open("/proc/uptime", O_RDONLY);
		if(0 > fd)
		{
			DEBUG_MSG("open /proc/uptime error\r\n");
			exit(1);
		}
		memset(buf, 0, 1024);
		len = read(fd, buf, 1024);
		if(0 < len)
		{
			DEBUG_MSG("uptime:%s", buf);
		}
		close(fd);
		sleep(1);
	}
	
	return 0;
}
