/* name: server.c
 * description: 套接字服务端测试程序
 * author: zuokongxiao
 * date: 2014-08-11
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "deftype.h"
#include "debug.h"

/* 主函数 */
int main(int argc, char *argv[])
{
    int res = 0;
    char *filedir;
    struct sockaddr_in serveradr;
    struct sockaddr_in clientadr;
    int socketfd = 0, clientsfd = 0;
    int server_len = 0, client_len = 0;
    char ch;

    /* 输入参数检查 */
    if(1 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        exit(0);
    }

    /* 创建套接字 */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socketfd)
    {
        DEBUG_MSG("E:failed to create socket!\r\n");
        exit(0);
    }
    else
    {
        DEBUG_MSG("D:create Socket Despcritor sucessfully.\r\n");
    }

    /* 套接字地址 */
    serveradr.sin_family = AF_INET;
    serveradr.sin_port = htons(5000); //网络字节序
    serveradr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_len = sizeof(serveradr);
    /* 命名套接字 */
    if(-1 == bind(socketfd, (struct sockaddr *)&serveradr, server_len))
    {
        DEBUG_MSG("E:failed to bind Port 5000.\r\n");
        exit(0);
    }
    else
    {
        DEBUG_MSG("D:bind the Port 5000 sucessfully.\r\n");
    }

    /* 创建套接字队列 */
    if(-1 == listen(socketfd, 10))
    {
        DEBUG_MSG("E:failed to listen Port 5000.\n\r\n");
        exit(0);
    }
    else
    {
        DEBUG_MSG("D:listening the Port 5000 sucessfully.\r\n");
    }

    while(1)
    {
        client_len = sizeof(clientadr);
        /* 等待套接字建立 */
        clientsfd = accept(socketfd, (struct sockaddr *)&clientadr, &client_len);
        if(-1 == clientsfd)
        {
            DEBUG_MSG("E:Obtain new Socket Despcritor error.\r\n");
            continue;
        }
        else
        {
            DEBUG_MSG("D:Server has got connect from %s.\r\n", inet_ntoa(clientadr.sin_addr));
        }

        read(clientsfd, &ch, 1);
        ch++;
        write(clientsfd, &ch, 1);
        close(clientsfd);
    }

    exit(0);
}



