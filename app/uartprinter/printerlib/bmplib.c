/* name: bmplib.c
* description: bmp
* author: zuokongxiao
* date: 2014-10-23
*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "deftype.h"
#include "debug.h"
#include "bmplib.h"


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

/* BMP文件头结构体定义 */
typedef struct tagBITMAPFILEHEADER
{
    U16 bfType; /* 类型(2 bytes)BM */
    U32 bfSize; /* 文件大小(4 bytes) */
    U16 bfReserved1; /* 保留 */
    U16 bfReserved2; /* 保留 */
    U32 bfOffBits; /* 文件头到图像数据的偏移值 */
} BITMAPFILEHEADER;

/* BMP文件信息头结构体定义 */
typedef struct tagBITMAPINFOHEADER
{
    U32 biSize; /* 此结构占用的字节数(4 bytes) */
    U32 biWidth; /* 图像宽度 */
    U32 biHeight; /* 图像高度 */
    U16 biPlanes; /* 颜色平面数，始终为1 */
    U16 biBitCount; /* 比特数/像素：1、4、8、16、24、32 */
    U32 biCompression; /* 压缩类型 */
    U32 biSizeImage; /* 图像大小 */
    U32 biXPelsPerMeter; /* 水平分辨率 */
    U32 biYPelsPerMeter; /* 垂直分辨率 */
    U32 biClrUsed; /* 颜色索引数 */
    U32 biClrImportant; /* 对图像显示有重要影响的颜色索引的数目 */
} BITMAPINFOHEADER;

/* 调色板结构体定义 */
typedef struct tagRGBQUAD
{
    U8 rgbBlue;
    U8 rgbGreen;
    U8 rgbRed;
    U8 rgbReserved;
} RGBQUAD;


/* 打印bmp文件头信息 */
static void print_bmp_file_header(BITMAPFILEHEADER *pstBmpFileHeader)
{
    /* 输入参数检查 */
    if(INVALID_POINTER(pstBmpFileHeader))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return;
    }
    DEBUG_MSG("D:bmp file header > type=0x%04x, size=%d, offbits=%d\r\n", \
              pstBmpFileHeader->bfType, \
              pstBmpFileHeader->bfSize, \
              pstBmpFileHeader->bfOffBits);
}

/* 打印位图信息头 */
static void print_bmp_info_header(BITMAPINFOHEADER *pstBmpInfoHeader)
{
    /* 输入参数检查 */
    if(INVALID_POINTER(pstBmpInfoHeader))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return;
    }
    DEBUG_MSG("D:bmp info header > size=%d, width=%d, height=%d, bitcount=%d, \
biCompression=%d, biSizeImage=%d, biXPelsPerMeter=%d, biYPelsPerMeter=%d, \
biClrUsed=%d, biClrImportant=%d\r\n", \
           pstBmpInfoHeader->biSize, \
           pstBmpInfoHeader->biWidth, \
           pstBmpInfoHeader->biHeight, \
           pstBmpInfoHeader->biBitCount, \
           pstBmpInfoHeader->biCompression, \
           pstBmpInfoHeader->biSizeImage, \
           pstBmpInfoHeader->biXPelsPerMeter, \
           pstBmpInfoHeader->biYPelsPerMeter, \
           pstBmpInfoHeader->biClrUsed, \
           pstBmpInfoHeader->biClrImportant);
}


/* 获取BMP文件的位图数据(打开1bit/像素) */
int get_bmp_data(bmp_t *pstBmp, const char *pcFileName)
{
    FILE *fp;
    BITMAPFILEHEADER stBmpFileHeader;
    BITMAPINFOHEADER stBmpInfoHeader;
    
    /* 输入参数检查 */
    if(INVALID_POINTER(pstBmp) || INVALID_POINTER(pcFileName))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    
    /* 打开bmp文件 */
    if((fp = fopen(pcFileName, "rb")) == NULL)
    {
        DEBUG_MSG("E:Error opening file %s.\r\n", pcFileName);
        return -1;
    }
    /* 读BMP文件头信息(分开读取) */
    fread(&(stBmpFileHeader.bfType), sizeof(stBmpFileHeader.bfType), 1, fp);
    fread(&(stBmpFileHeader.bfSize), sizeof(stBmpFileHeader.bfSize), 1, fp);
    fread(&(stBmpFileHeader.bfReserved1), sizeof(stBmpFileHeader.bfReserved1), 1, fp);
    fread(&(stBmpFileHeader.bfReserved2), sizeof(stBmpFileHeader.bfReserved2), 1, fp);
    fread(&(stBmpFileHeader.bfOffBits), sizeof(stBmpFileHeader.bfOffBits), 1, fp);
    /* 打印文件头信息 */
    print_bmp_file_header(&stBmpFileHeader);
    if(0x4d42 != stBmpFileHeader.bfType)
    {
        DEBUG_MSG("E:is not bmp file.\r\n");
        return -1;
    }
    /* 读位图信息 */
    if(0 > fread(&stBmpInfoHeader, sizeof(stBmpInfoHeader), 1, fp))
    {
        ERROR_MSG("E:fread bmiHeader error!\r\n");
    }
    /* 打印位图信息头 */
    print_bmp_info_header(&stBmpInfoHeader);
    
    /* 保存信息 */
    pstBmp->usBitCount = stBmpInfoHeader.biBitCount;  
    pstBmp->ulWidth = stBmpInfoHeader.biWidth;
    pstBmp->ulHeight = stBmpInfoHeader.biHeight;
    pstBmp->ulRowBytes = (((pstBmp->ulWidth)*(pstBmp->usBitCount) + 31) >> 5) << 2;
    pstBmp->pucData = (unsigned char *)malloc((pstBmp->ulHeight)*(pstBmp->ulRowBytes));  
    if(NULL == pstBmp->pucData)  
    {
        DEBUG_MSG("E:malloc error.\r\n");
        return -1;
    }
    fseek(fp, stBmpFileHeader.bfOffBits, SEEK_SET);
    fread(pstBmp->pucData, (pstBmp->ulHeight)*(pstBmp->ulRowBytes), 1, fp); 
    /* 关闭文件 */
    fclose(fp);  
       
    return 0;
}

/* 释放GetBmpData分配的空间 */
void free_bmp_data(unsigned char *pucData)
{  
    if(pucData)
    {  
        free(pucData);
        pucData = NULL;
    }
}

