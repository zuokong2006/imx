/* name: printerlib.c
* description: 串口打印机库(RG-E487串口热敏打印机)
* author: zuokongxiao
* date: 2014-10-23
*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "deftype.h"
#include "debug.h"
#include "bmplib.h"

/* 命令字符定义 */
#define CHAR_ESC        0x1b
#define CHAR_LF         0x0a
#define CHAR_CR         0x0d
#define CHAR_NUL        0x00

/* 位定义 */
const unsigned char aucByteBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* 打印机文件描述符 */
int lPrinterFd = 0;


/* 读写接口 */
void printer_fildes(int fd)
{
    lPrinterFd = fd;
}

/* 设置行间距 */
int printer_set_linespacing(unsigned char ucLineNum)
{
    unsigned char aucBuf[3] = {CHAR_ESC, '1', 0x00};
    
    aucBuf[2] = ucLineNum;
    return write(lPrinterFd, aucBuf, 3);
}

/* 图形打印 */
int printer_picture_print(unsigned char *pucBuf, int lSize)
{
    unsigned char aucBuf[390] = {CHAR_ESC, 'K'};
    
    /* 输入参数检查 */
    if(INVALID_POINTER(pucBuf) || (384 < lSize))
    {
        DEBUG_MSG("E:input param error.\r\n");
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
#if 0
    for(i=0; i<(lSize+6); i++)
    {
        printf("%02x ", aucBuf[i]);
    }
    printf("\r\n");
#endif
    /* 写数据 */
    //printer_set_linespacing(0);
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
    //return write(lPrinterFd, aucBuf, lSize+5);
}

/* bmp图形打印 */
int printer_bmp_print(char *pcBmpName)
{
    unsigned char ucBitCount = 0;
    unsigned int ulWidth = 0, ulHeight = 0, ulRow = 0;
    unsigned char *pData;
    unsigned int i = 0, j = 0;
    unsigned char ucBit = 0;
    
    /* 输入参数检查 */
    if(INVALID_POINTER(pcBmpName))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    /* 行间距设置为0 */
    printer_set_linespacing(0);
    /* 获取 */
    pData = GetBmpData(&ucBitCount, &ulWidth, &ulHeight, pcBmpName);
    if(NULL == pData)  
    {  
        DEBUG_MSG("E:get bmp file data error.\r\n");
        return -1;
    }
    if(1 != ucBitCount)
    {
        DEBUG_MSG("E:ucBitCount != 1.\r\n");
        return -1;
    }
    
    ulRow = (ulWidth + 7) / 8;
    DEBUG_MSG("D:ulRow=%d.\r\n", ulRow);
    unsigned char *pucBuf = (unsigned char *)malloc(ulWidth);
    unsigned char ucTemp = 0, ucDot = 0;
    unsigned int ulRowCnt = 0;
    for(j=ulHeight; j!=0; j--)
    {
        for(i=0; i<ulRow; i++)
        {
            printf("%02x ", *(pData+(j-1)*ulRow+i));
        }
        printf("\r\n");
    }
    printf("\r\n");
    
    ucBit = 8;
    memset(pucBuf, 0, ulWidth);
    for(j=ulHeight; j!=0; j--)
    {
        ulRowCnt = 0;
        for(i=0; i<ulRow; i++)
        {
            ucTemp = *(pData+(j-1)*ulRow+i);
            for(ucDot=8; ucDot>0; ucDot--)
            {
                if(ulRowCnt < ulWidth)
                {
                    if(aucByteBit[ucDot-1] != (aucByteBit[ucDot-1] & ucTemp))
                    {
                        *(pucBuf+ulRowCnt) |= (aucByteBit[ucBit-1]);
                    }
                }
                else
                {
                    break;
                }
                ulRowCnt++;
            }
        }
        
        ucBit--;
        if(0 == ucBit)
        {
            printer_picture_print(pucBuf, ulWidth);
            ucBit = 8;
            memset(pucBuf, 0, ulWidth);
        }
    }
    if((0 < ucBit) && (8 > ucBit))
    {
        printer_picture_print(pucBuf, ulWidth);
    }
    
    /* 释放内存 */
    free(pucBuf);
    FreeBmpData(pData);  
    
    return 0;
}






