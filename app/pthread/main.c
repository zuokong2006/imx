
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "deftype.h"
#include "debug.h"
#include "test.h"

/* 字符串缓冲区长度定义 */
#define BUF_SIZE        (1024)

/* 信号量 */
sem_t bin_msg;
/* 互斥量 */
pthread_mutex_t buf_mutex;

int main(int argc, char *argv[])
{
    int res = 0;
    pthread_t thread_pid = 0;
    void *thread_result;
    char *buf;
    struct arg_st stArg;

    DEBUG_MSG("start main function!\r\n");
    errno = 2;
    ERROR_MSG("errno msg test!\r\n");
    /* 创建二进制信号量 */
    res = sem_init(&bin_msg, 0, 0);
    if(0 != res)
    {
        perror("sem_init failed");
        exit(-1);
    }
    DEBUG_MSG("sem_init ok!\r\n");
    /* 初始化互斥量 */
    res = pthread_mutex_init(&buf_mutex, NULL);
    if(0 != res)
    {
        perror("pthread_mutex_init failed");
        exit(-1);
    }
    DEBUG_MSG("pthread_mutex_init ok!\r\n");
    /* 分配内存 */
    buf = malloc(BUF_SIZE);
    if(NULL == buf)
    {
        perror("malloc failed");
        exit(-1);
    }
    memset(buf, 0, BUF_SIZE);
    stArg.pMsg = &bin_msg;
    stArg.pMutex = &buf_mutex;
    stArg.pBuf = buf;
    /* 创建线程 */
    res = pthread_create(&thread_pid, NULL, thread_test, &stArg);
    if(0 != res)
    {
        free(buf);
        perror("thread create failed");
        exit(-1);
    }
    DEBUG_MSG("thread_create is ok!\r\n");
    /* 信号量同步 */
    while(0 != strncmp("exit", buf, 4))
    {
        fgets(buf, BUF_SIZE, stdin);
        DEBUG_MSG("input:%s", buf);
        sem_post(&bin_msg);
    }
    /* pthread_join等待线程返回 */
    res = pthread_join(thread_pid, &thread_result);
    if(0 != res)
    {
        free(buf);
        perror("thread join failed");
        exit(-1);
    }
    DEBUG_MSG("thread join, it returned %s\r\n", (int8 *)thread_result);
    /* 销毁信号量 */
    sem_destroy(&bin_msg);
    pthread_mutex_destroy(&buf_mutex);
    free(buf);

    exit(0);
}



