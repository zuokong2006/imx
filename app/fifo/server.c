#include <ctype.h>
#include "common.h"
#include "deftype.h"
#include "debug.h"


int main(int argc, char *argv[])
{
    int server_fifo_fd, client_fifo_fd;
    struct fifo_buf_st fifo_buf;
    char client_fifo_name[256];
    int ret;
    char *tmp_char_ptr;

    /* 创建server fifo */
    if(-1 == mkfifo(SERVER_FIFO_NAME, 0777))
    {
        ERROR_MSG("server fifo mkfifo error!\r\n");
        exit(-1);
    }
    /* open被阻塞 */
    server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
    if(-1 == server_fifo_fd)
    {
        ERROR_MSG("open server fifo error!\r\n");
        exit(-1);
    }

    DEBUG_MSG("open server fifo ok!\r\n");
    //sleep(10);

    do
    {
        ret = read(server_fifo_fd, &fifo_buf, sizeof(fifo_buf));
        if(0 < ret)
        {
            /* 转换为大写 */
            tmp_char_ptr = fifo_buf.buf;
            while(*tmp_char_ptr)
            {
                *tmp_char_ptr = toupper(*tmp_char_ptr);
                tmp_char_ptr++;
            }
            DEBUG_MSG("server send:%s\r\n", fifo_buf.buf);
            /* 打开client fifo */
            sprintf(client_fifo_name, CLIENT_FIFO_NAME, fifo_buf.client_pid);
            client_fifo_fd = open(client_fifo_name, O_WRONLY);
            if(-1 == client_fifo_fd)
            {
                ERROR_MSG("open client fifo error!\r\n");
            }
            else
            {
                write(client_fifo_fd, &fifo_buf, sizeof(fifo_buf));
                close(client_fifo_fd);
            }
        }
    } while(0 < ret);

    close(server_fifo_fd);
    unlink(SERVER_FIFO_NAME);
    exit(0);
}

