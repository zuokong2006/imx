
#include <ctype.h>
#include "common.h"
#include "deftype.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    int server_fifo_fd, client_fifo_fd;
    struct fifo_buf_st fifo_buf;
    int i = 0;
    char client_fifo_name[256];

    /* 打开server_fifo */
    server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
    if(-1 == server_fifo_fd)
    {
        ERROR_MSG("server fifo open error!\r\n");
        exit(-1);
    }
    /* 创建client fifo */
    fifo_buf.client_pid = getpid();
    sprintf(client_fifo_name, CLIENT_FIFO_NAME, fifo_buf.client_pid);
    if(-1 == mkfifo(client_fifo_name, 0777))
    {
        ERROR_MSG("mkfifo error(client_fifo=%d)!\r\n", fifo_buf.client_pid);
        exit(-1);
    }

    /* 发送数据到server fifo */
    for(i = 0; i < 10; i++)
    {
        /* 准备数据 */
        //fifo_buf.client_pid = getpid();
        sprintf(fifo_buf.buf, "client=%d data(%d)!", fifo_buf.client_pid, i);
        DEBUG_MSG("client send:%s\r\n", fifo_buf.buf);
        /* 写server fifo */
        if(-1 == write(server_fifo_fd, &fifo_buf, sizeof(fifo_buf)))
        {
            ERROR_MSG("write server fifo error!\r\n");
            break;
        }
        /* 读取client fifo(如果server为准备就绪open、read会被阻塞) */
        client_fifo_fd = open(client_fifo_name, O_RDONLY);
        if(-1 == client_fifo_fd)
        {
            ERROR_MSG("open client fifo error!\r\n");
            break;
        }
        if(0 < read(client_fifo_fd, &fifo_buf, sizeof(fifo_buf)))
        {
            DEBUG_MSG("server ret:%s\r\n", fifo_buf.buf);
        }
        close(client_fifo_fd);
    }

    /*  */
    close(server_fifo_fd);
    unlink(client_fifo_name);
    exit(0);
}


