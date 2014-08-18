
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>

#define WR_PIPE		1
#define RD_PIPE		0

static int pfd[2] = {0};

void tell_parent(pid_t pid)
{
	if(1 != write(pfd[WR_PIPE], "c", 1))
	{
		printf("write pipe error!\r\n");
		//exit(1);
	}
}

void wait_child(void)
{
	char c;
	
	if(1 != read(pfd[RD_PIPE], &c, 1))
	{
		printf("read pipe error!\r\n");
		//exit(1);
	}
	if('c' != c)
	{
		printf("data error!\r\n");
	}
}

int main(void) 
{
	pid_t pid;
	
	printf("pipe test!\r\n");
	if(0 > pipe(pfd))
	{
		printf("create pipe error!\r\n");
	}
	
	pid = fork();
	if(0 > pid)
		printf("fork error!\r\n");
	else if(0 == pid)
	{
		printf("child process, id=%d!\r\n", getpid());
		printf("tell parent!\r\n");
		tell_parent(getpid());
		exit(0);
	}
	else
	{
		printf("parent process, pid=%d id=%d!\r\n", pid, getpid());
		printf("wait child!\r\n");
		wait_child();
		printf("wait child is ok!\r\n");
		exit(0);
	}
}
