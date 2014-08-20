/* name: client.c
 * description: 套接字客户端测试程序
 * author: zuokongxiao
 * date: 2014-08-12
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
    struct sockaddr_in clientadr;
    int clientsfd = 0;
    int client_len = 0;
    char ch = 'T';

    /* 输入参数检查 */
    if(1 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        exit(0);
    }

    /* 创建套接字 */
    clientsfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientsfd)
    {
        DEBUG_MSG("E:failed to create socket!\r\n");
        exit(0);
    }
    else
    {
        DEBUG_MSG("D:create Socket Despcritor sucessfully.\r\n");
    }

    /* 套接字地址 */
    clientadr.sin_family = AF_INET;
    clientadr.sin_port = htons(5000); //网络字节序
    inet_pton(AF_INET, "127.0.0.1", &clientadr.sin_addr);
    //clientadr.sin_addr.s_addr = htonl(inet_addr("127.0.0.1"));
    
    client_len = sizeof(clientadr);
    /* 等待套接字建立 */
    res = connect(clientsfd, (struct sockaddr *)&clientadr, client_len);
    if(-1 == res)
    {
        DEBUG_MSG("E:Obtain new Socket Despcritor error.\r\n");
        exit(0);
    }
    else
    {
        DEBUG_MSG("D:Server has got connect from %s.\r\n", inet_ntoa(clientadr.sin_addr));
    }

    write(clientsfd, &ch, 1);
    read(clientsfd, &ch, 1);
    DEBUG_MSG("D:char from server = %c.\r\n", ch);
    close(clientsfd);

    exit(0);
}



