
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
	struct tm stTime = {0};
	int i;
	pid_t pid;
	
	pid = fork();
	if(0 > pid)
		printf("fork error!\r\n");
	else if(0 == pid)
		printf("child process, id=%d!\r\n", getpid());
	else
		printf("parent process, pid=%d id=%d!\r\n", pid, getpid());
	
	for(i = 0; i < 5; i++) {
		printf("pid=%d, hello word:%d \n", getpid(), i);
	}
}
