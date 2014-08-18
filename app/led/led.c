
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd = 0, i = 0, ret = 0;
	
	fd = open("/dev/bb_led", O_RDWR);
	printf("fd=%d\r\n", fd);
	if(0 > fd)
	{
		perror("open /dev/bb_led");
		exit(1);
	}
	
	for(i=0; i<10; i++)
	{
		ret = ioctl(fd, i%2);
		printf("ioctl ret = %d\r\n", ret);
		sleep(1);
	}
	
	close(fd);
	
	return 0;
}
