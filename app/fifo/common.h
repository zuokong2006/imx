
#ifndef _COMMON_H
#define _COMMON_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/* fifo名称定义 */
#define SERVER_FIFO_NAME    "/tmp/server_fifo"
#define CLIENT_FIFO_NAME    "/tmp/client_%d_fifo"
/* 缓冲区大小定义 */
#define BUFFER_SIZE 20

/* fifo缓冲区 */
struct fifo_buf_st
{
    pid_t client_pid; /* pid */
    char buf[BUFFER_SIZE - 1];
};

#endif /* _COMMON_H */

