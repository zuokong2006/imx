/* name: uartlog.c
 * description: 串口数据保存
 * author: zuokongxiao
 * date: 2014-10-20
 */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include "deftype.h"
#include "debug.h"

/* 缓冲区大小定义 */
#define BUF_SIZE        (10240)
/* 串口波特率定义 */
#define BAUDRATE        B115200

/* 文件名称 */
static char acFileName[64];
/* 缓冲区写索引 */
static int lWrIndex = 0;
/* 缓冲区定义 */
static char acBuf[BUF_SIZE];


/* 打开串口 */
static int open_uart(char *pcSerialName)
{
    int iFd;
    struct termios opt; 

    DEBUG_MSG("D:name=%s\r\n", pcSerialName);
    /* 打开串口 */
    iFd = open(pcSerialName, O_RDWR|O_NOCTTY|O_NDELAY);
    if(iFd < 0)
    {
        ERROR_MSG("E:open error!\r\n");
        return -1;
    }
    /* 获取设备termios结构 */
    if(0 > tcgetattr(iFd, &opt))
    {
        ERROR_MSG("E:tcgetattr error!\r\n");
        return -1;
    }
    /* 设置波特率 */
    cfsetispeed(&opt, BAUDRATE);
    cfsetospeed(&opt, BAUDRATE);
    /* 控制标志：8位数据位、无校验位 */
    opt.c_cflag &= ~(CSTOPB | CSIZE | PARENB);
    opt.c_cflag |= CS8;
    opt.c_cflag |= (CLOCAL | CREAD);
    /* 输入标志 */
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* 输出标志 */
    opt.c_oflag &= ~(OPOST);
    /* 本地标志：原始模式 */
    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* VMIN:需读取的最小字节数 VTIME:限定的传输或等待的最长时间 */
    opt.c_cc[VMIN] = 16; //一次read调用期望返回的最小字节数
    opt.c_cc[VTIME] = 2; //150表示15秒
    /* 设置 */
    if(0 > tcsetattr(iFd, TCSANOW, &opt))
    {
        ERROR_MSG("E:tcsetattr error!\r\n");
        return -1;
    }

    return iFd;
}

/* 创建uartlog文件 */
static int create_uartlog_file(char *pcFileName)
{
    struct timeval cur_time;
    struct tm *pstTm;
    int fd = 0, len = 0;
    int ret = 0;
    
    /* 输入参数检查 */
    if(INVALID_POINTER(pcFileName))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* 获取当前时间并转换为文件名 */
    gettimeofday(&cur_time, NULL);
    pstTm = localtime(&(cur_time.tv_sec));
    sprintf(pcFileName, "/home/uartlog_%04d%02d%02d%02d%02d%02d.txt", \
            pstTm->tm_year+1900, pstTm->tm_mon+1, pstTm->tm_mday, \
            pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
    /* 创建文件 */
    fd = open(pcFileName, O_CREAT|O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
    if(0 > fd)
    {
        ERROR_MSG("create uart log file error\r\n");
        return -1;
    }
    /* 写文件 */
    len = write(fd, "create log file is ok.\r\n", 24);
    if(len < 24)
    {
        ERROR_MSG("write file len < 24!\r\n");
        ret = -1;
    }
    else
    {
        DEBUG_MSG("write file ok!\r\n");
    }
    /* 关闭文件 */
    close(fd);
    
    return ret;
}

/* 写uartlog文件 */
static int write_uartlog_file(char *pcFileName, char *pcBuf, int lSize)
{
    int fd = 0, len = 0;
    int ret = 0;
    
    /* 输入参数检查 */
    if(INVALID_POINTER(pcFileName) || INVALID_POINTER(pcBuf))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* 打开文件 */
    fd = open(pcFileName, O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
    if(0 > fd)
    {
        ERROR_MSG("open uart log file error\r\n");
        return -1;
    }
    /* 写文件 */
    len = write(fd, pcBuf, lSize);
    if(len < 24)
    {
        ERROR_MSG("write file len < lSize!\r\n");
        ret = -1;
    }
    else
    {
        //DEBUG_MSG("write file ok!\r\n");
    }
    /* 关闭文件 */
    close(fd);
    
    return ret;
}

/* alrm信号处理函数 */
void sigalrm_fn(int sig)
{
    if(0 < lWrIndex)
    {
        write_uartlog_file(acFileName, acBuf, lWrIndex);
        lWrIndex = 0;
    }
}

/* 
 * 主函数 
 * 用法：uartlog + 串口设备名"/dev/ttySAC1"
 */
int main(int argc, char *argv[])
{
    int uartfd;
    int i = 0, len = 0;
    char tmp[32];
    
    /* 输入参数检查 */
    if(2 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    DEBUG_MSG("D:dev name = %s!\r\n", argv[1]);
    /* 打开串口 */
    uartfd = open_uart(argv[1]);
    if(0 > uartfd)
    {
        DEBUG_MSG("D:open uart error!\r\n");
        return -1;
    }
    /* 创建log文件 */
    if(0 > create_uartlog_file(acFileName))
    {
        DEBUG_MSG("D:create log file error!\r\n");
        close(uartfd);
        return -1;
    }
    /* 信号 */
    signal(SIGALRM, sigalrm_fn);
    
    /* 主循环 */
    while(1)
    {
        len = read(uartfd, tmp, 16);
        for(i=0; i<len; i++)
        {
            alarm(5);
            printf("%c", tmp[i]);
            acBuf[lWrIndex] = tmp[i];
            lWrIndex++;
            if(BUF_SIZE <= lWrIndex)
            {
                lWrIndex = 0;
                write_uartlog_file(acFileName, acBuf, BUF_SIZE);
            }
        }
    } /* end of while(1)... */
}

