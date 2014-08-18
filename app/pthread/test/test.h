
#ifndef _TEST_H
#define _TEST_H

//#include <semaphore.h>

/* 参数结构体定义 */
struct arg_st
{
    sem_t *pMsg;
    pthread_mutex_t *pMutex;
    char *pBuf;
};

void *thread_test(void *arg);

#endif
