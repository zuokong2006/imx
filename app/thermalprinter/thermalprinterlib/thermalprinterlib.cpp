/* name: printerlib.c
* description: 串口打印机库(RG-E487串口热敏打印机)
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
#include "EasyBMP.h"
#include "thermalprinterlib.h"

//using namespace std;

/* 命令字符定义 */
#define CHAR_ESC                0x1b
#define CHAR_LF                 0x0a
#define CHAR_CR                 0x0d
#define CHAR_NUL                0x00
/* 打印机最大宽度定义 */
#define PRINTER_WIDTH_SIZE      384
/* 串口波特率定义 */
#define BAUDRATE                B57600

/* 位定义 */
const unsigned char aucByteBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};


/* 打开串口 */
static int open_uart(const char *pcSerialName)
{
    int iFd;
    struct termios opt;

    printf("D:name=%s\r\n", pcSerialName);
    /* 打开串口 */
    iFd = open(pcSerialName, O_RDWR | O_NOCTTY | O_NDELAY);
    if(iFd < 0)
    {
        printf("E:open error!\r\n");
        return -1;
    }
    /* 获取设备termios结构 */
    if(0 > tcgetattr(iFd, &opt))
    {
        printf("E:tcgetattr error!\r\n");
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
        printf("E:tcsetattr error!\r\n");
        return -1;
    }

    return iFd;
}




/* 打开设备 */
int thermalprinter::open_dev(const char *pcTtyName)
{
    if(NULL == pcTtyName)
    {
        printf("E:input param error.\r\n");
        return -1;
    }
    /* 打开串口 */
    lPrinterFd = open_uart(pcTtyName);
    if(0 > lPrinterFd)
    {
        printf("D:open uart error!\r\n");
        return -1;
    }

    return 0;
}

/* 关闭设备 */
int thermalprinter::close_dev(void)
{
    close(lPrinterFd);

    return 0;
}

/* 初始化打印机 */
int thermalprinter::init(void)
{
    unsigned char aucBuf[3] = {CHAR_ESC, 0x40, 0x00};
    
    return write(lPrinterFd, aucBuf, 3);
}

/* 设置行间距 */
int thermalprinter::set_linespacing(unsigned char ucLineNum)
{
    unsigned char aucBuf[3] = {CHAR_ESC, '1', 0x00};
    
    aucBuf[2] = ucLineNum;
    return write(lPrinterFd, aucBuf, 3);
}

/* 执行指定行走纸 */
int thermalprinter::paper_feed(unsigned char ucLineNum)
{
    unsigned char aucBuf[3] = {CHAR_ESC, 'J', 0x00};
    
    aucBuf[2] = ucLineNum;
    return write(lPrinterFd, aucBuf, 3);
}

/* 获取打印机状态 */
int thermalprinter::get_state(unsigned char *pucState)
{
    unsigned char aucBuf[3] = {0x10, 0x04, 0x01};
    
    /* 输入参数检查 */
    if(NULL == pucState)
    {
        printf("E:input param error.\r\n");
        return -1;
    }
    /* 传输打印机状态 */
    if(0 > write(lPrinterFd, aucBuf, 3))
    {
        printf("E:write cmd error(get printer state)!\r\n");
        return -1;
    }
    /* 获取状态 */
    if(1 != read(lPrinterFd, aucBuf, 1))
    {
        printf("E:read serial error!\r\n");
        return -1;
    }
    /* 解析返回数据 */
    if(0x12 != (aucBuf[0] & 0x97))
    {
        printf("E:return data error!\r\n");
        return -1;
    }
    //printf("D:printer state = 0x%02x.\r\n", aucBuf[0]);
    *pucState = PRINTER_NORMAL;
    if(0x12 == aucBuf[0])
    {
        //printf("D:printer normal.\r\n");
    }
    else
    {
        /* 联机状态 */
        if(PRINTER_OFFLINE == (aucBuf[0]&PRINTER_OFFLINE))
        {
            printf("D:printer offline.\r\n");
            *pucState |= PRINTER_OFFLINE;
        }
        /* 是否缺纸 */
        if(PRINTER_OUT_OF_PAPER == (aucBuf[0]&PRINTER_OUT_OF_PAPER))
        {
            printf("D:printer out of paper.\r\n");
            *pucState |= PRINTER_OUT_OF_PAPER;
        }
        /* 机头状态 */
        if(PRINTER_HEAD_OVERHEAT == (aucBuf[0]&PRINTER_HEAD_OVERHEAT))
        {
            printf("D:printer head overhead.\r\n");
            *pucState |= PRINTER_HEAD_OVERHEAT;
        }
    }
    
    return 0;
}

/* 图形打印 */
int thermalprinter::print_picture(unsigned char *pucBuf, int lSize)
{
    unsigned char aucBuf[390] = {CHAR_ESC, 'K'};
    unsigned char ucState = 0;
    
    /* 输入参数检查 */
    if((NULL == pucBuf) || (PRINTER_WIDTH_SIZE < lSize))
    {
        printf("E:input param error.\r\n");
        return -1;
    }
    /* 检测打印机状态 */
    if(0 > get_state(&ucState))
    {
        printf("E:get printer state error.\r\n");
        return -1;
    }
    if(PRINTER_NORMAL != ucState)
    {
        printf("E:printer abnormal.\r\n");
        return -1;
    }

    /* 点数 */
    aucBuf[2] = (unsigned char)lSize;
    aucBuf[3] = (unsigned char)(lSize>>8);
    /* 数据 */
    memcpy(&(aucBuf[4]), pucBuf, lSize);
    aucBuf[4+lSize] = CHAR_LF;
    aucBuf[4+lSize+1] = CHAR_CR;

    unsigned int i = 0;
    unsigned char n = 0;
    /* 写数据 */
    unsigned int ulCnt = (lSize+6+15)/16 - 1;
    for(i=0; i<ulCnt; i++)
    {
        write(lPrinterFd, (aucBuf+i*16), 16);
    }
    n = (lSize+6)%16;
    if(0 == n)
    {
        write(lPrinterFd, (aucBuf+ulCnt*16), 16);
    }
    else
    {
        write(lPrinterFd, (aucBuf+ulCnt*16), n);
    }
    
    return 0;
}

/* 1位bmp图形打印 */
int thermalprinter::print_1bit_bmp(const char *pcBmpName)
{
    int res = 0;
    
    /* 输入参数检查 */
    if(NULL == pcBmpName)
    {
        printf("E:input param error.\r\n");
        return -1;
    }
    /* 行间距设置为0 */
    set_linespacing(0);

    /* easybmp类 */
    BMP cBmp;
    /* 读取bmp文件数据 */
    if(false == cBmp.ReadFromFile(pcBmpName))
    {
        printf("E:read bmp file data error.\r\n");
        return -1;
    }
    if((1 != cBmp.TellBitDepth()) || (0 > cBmp.TellHeight()))
    {
        printf("E:bmp iamge data format is error.\r\n");
        return -1;
    }
    //printf("D:Height=%d Width=%d.\r\n", cBmp.TellHeight(), cBmp.TellWidth());
    
    /* 解析图像数据 */
    unsigned char *pucBuf;
    unsigned char ucTemp = 0, ucBit = 0;
    unsigned int ulRowCnt = 0, i = 0, j = 0, ulPrintWidth = 0;
    ulPrintWidth = cBmp.TellWidth();
    if(PRINTER_WIDTH_SIZE < cBmp.TellWidth())
    {
        ulPrintWidth = PRINTER_WIDTH_SIZE;
    }
    pucBuf = (unsigned char *)malloc(ulPrintWidth);
#if 0 //打印图像数据
    for(j=0; j<cBmp.TellHeight(); j++)
    {
        for(i = 0; i<cBmp.TellWidth(); i++)
        {
            printf("%02x ", cBmp(i, j)->Red);
        }
        printf("\r\n");
    }
    printf("\r\n");
#endif

    ucBit = 8;
    memset(pucBuf, 0, ulPrintWidth);
    for(j = 0; j < cBmp.TellHeight(); j++)
    {
        /* 图像行处理 */
        ulRowCnt = 0;
        for(i = 0; i<ulPrintWidth; i++) //像素点存放相反
        {
            ucTemp = cBmp(i, j)->Red;
            if(0 == ucTemp)
            {
                *(pucBuf + ulRowCnt) |= (aucByteBit[ucBit - 1]);
            }
            ulRowCnt++;
        }
        ucBit--;
        /* 打印图形 */
        if(0 == ucBit)
        {
            ucBit = 8;
            if(0 > print_picture(pucBuf, ulPrintWidth))
            {
                printf("E:picture print error1.\r\n");
                /* 需清空缓冲区 */
                res = -1;
                break;
            }
            memset(pucBuf, 0, ulPrintWidth);
        }
    }
    /* 打印剩余图形 */
    if((0 < ucBit) && (8 > ucBit))
    {
        if(0 > print_picture(pucBuf, ulPrintWidth))
        {
            printf("E:picture print error2.\r\n");
            /* 需清空缓冲区 */
            res = -1;
        }
    }
    /* 释放内存 */
    free(pucBuf);

    return res;
}



