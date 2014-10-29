/* name: bmplib.h
 * description: BMP库
 * author: zuokongxiao
 * date: 2014-10-23
 */
#ifndef __BMPLIB_H
#define __BMPLIB_H


/* BMP文件结构体定义 */
typedef struct bmp_file_t
{
    unsigned int ulWidth; /* 图像宽度 */
    unsigned int ulHeight; /* 图像高度 */
    unsigned int ulRowBytes; /* 每行字节数 */
    unsigned short usBitCount; /* 位数/像素 */
    unsigned char *pucData; /* 指向图像数据 */
} bmp_t;


/* 获取BMP文件的位图数据 */
int get_bmp_data(bmp_t *pstBmp, const char *pcFileName);
/* 释放get_bmp_data分配的内存 */
void free_bmp_data(unsigned char *pucData);


#endif

