/* name: uartprinter.c
 * description: 串口打印机
 * author: zuokongxiao
 * date: 2014-10-23
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
#include "printerlib.h"
#include "bmplib.h"


/* 串口波特率定义 */
#define BAUDRATE        B9600


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

typedef struct _LI_RGB  
{  
    U8 b;  
    U8 g;  
    U8 r;  
}LI_RGB; 

#define WIDTH   10 
#define HEIGHT  10 

/* 
 * 主函数 
 * 用法：uartprinter + 串口设备名"/dev/ttySP0"
 */
int main(int argc, char *argv[])
{
    int uartfd;
    char tmp[32];
    
    /* 输入参数检查 */
    if(2 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    //DEBUG_MSG("D:dev name = %s!\r\n", argv[1]);
    /* 打开串口 */
    uartfd = open_uart(argv[1]);
    if(0 > uartfd)
    {
        DEBUG_MSG("D:open uart error!\r\n");
        return -1;
    }
    
    /* 设置串口打印机 */
    printer_fildes(uartfd);
    printer_set_linespacing(0);
    
    tmp[0] = 0x1b;
    tmp[1] = 0x4a;
    tmp[2] = 20;
    write(uartfd, tmp, 3);
    printer_bmp_print("in2.bmp");
    tmp[2] = 60;
    write(uartfd, tmp, 3);
    
    DEBUG_MSG("D:enter main loop.\r\n");
    
    /* 主循环 */
    while(1)
    {
        
    } /* end of while(1)... */
}

