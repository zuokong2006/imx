
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "deftype.h"
#include "debug.h"
#include "test.h"

void *thread_test(void *arg)
{
    struct arg_st *pstArg = (struct arg_st *)arg;

    DEBUG_MSG("thread_test function is running.\r\n");
    if(NULL == pstArg)
    {
        DEBUG_MSG("pstArg == NULL.\r\n");
        pthread_exit("arg is NULL");
    }
    /* 等待信号量 */
    sem_wait(pstArg->pMsg);
    while(0 != strncmp("exit", pstArg->pBuf, 4))
    {
        DEBUG_MSG("input %d characters.\r\n", strlen(pstArg->pBuf) - 1);
        sem_wait(pstArg->pMsg);
    }
    /* 线程退出 */
    sleep(3);
    pthread_exit("thank you for the cpu time.");
}

