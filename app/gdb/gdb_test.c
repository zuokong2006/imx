
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>

/* 函数声明 */
int sum(int m);

int main(int argc, char *argv[]) 
{
	int i = 0, count = 0, n = 0;
	
	/* 输入参数判断 */
	if(2 != argc)
	{
		fprintf(stderr, "usage: ./gdb_test count[int]\r\n");
		exit(1);
	}
	
	count = atoi(argv[1]);
	sum(count);
	n = 0;
	for(i=1; i<=20; i++)
	{
		n = n + i;
	}
	printf("the sum of 1-20 is %d\r\n", n);
	
	return 0;
}

int sum(int m)
{
	int i = 0, n = 0;
	
	for(i=1; i<=m; i++)
	{
		n += i;
	}
	printf("The sum of 1-%d is %d\r\n", m, n);
	
	return 0;
}
