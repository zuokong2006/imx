/* name: bmplib.c
* description: bmp
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

int GenBmpFile(U8 *pData, U8 bitCountPerPix, U32 width, U32 height, const char *filename)  
{  
    FILE *fp = fopen(filename, "wb");  
    if(!fp)  
    {  
        printf("fopen failed : %s, %d\n", __FILE__, __LINE__);  
        return 0;  
    }  
  
    U32 bmppitch = ((width*bitCountPerPix + 31) >> 5) << 2;  
    U32 filesize = bmppitch*height;  
  
    BITMAPFILE bmpfile;  
  
    bmpfile.bfHeader.bfType = 0x4D42;  
    bmpfile.bfHeader.bfSize = filesize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
    bmpfile.bfHeader.bfReserved1 = 0;  
    bmpfile.bfHeader.bfReserved2 = 0;  
    bmpfile.bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
  
    bmpfile.biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
    bmpfile.biInfo.bmiHeader.biWidth = width;  
    bmpfile.biInfo.bmiHeader.biHeight = height;  
    bmpfile.biInfo.bmiHeader.biPlanes = 1;  
    bmpfile.biInfo.bmiHeader.biBitCount = bitCountPerPix;  
    bmpfile.biInfo.bmiHeader.biCompression = 0;  
    bmpfile.biInfo.bmiHeader.biSizeImage = 0;  
    bmpfile.biInfo.bmiHeader.biXPelsPerMeter = 0;  
    bmpfile.biInfo.bmiHeader.biYPelsPerMeter = 0;  
    bmpfile.biInfo.bmiHeader.biClrUsed = 0;  
    bmpfile.biInfo.bmiHeader.biClrImportant = 0;  
  
    fwrite(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, fp);  
    fwrite(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, fp);  
  
    U8 *pEachLinBuf = (U8*)malloc(bmppitch);  
    memset(pEachLinBuf, 0, bmppitch);  
    U8 BytePerPix = bitCountPerPix >> 3;  
    U32 pitch = width * BytePerPix;  
    if(pEachLinBuf)  
    {  
        int h,w;  
        for(h = height-1; h >= 0; h--)  
        {  
            for(w = 0; w < width; w++)  
            {  
                //copy by a pixel  
                pEachLinBuf[w*BytePerPix+0] = pData[h*pitch + w*BytePerPix + 0];  
                pEachLinBuf[w*BytePerPix+1] = pData[h*pitch + w*BytePerPix + 1];  
                pEachLinBuf[w*BytePerPix+2] = pData[h*pitch + w*BytePerPix + 2];  
            }  
            fwrite(pEachLinBuf, bmppitch, 1, fp);  
              
        }  
        free(pEachLinBuf);  
    }  
  
    fclose(fp);  
  
    return 1;  
}  
  
//获取BMP文件的位图数据(无颜色表的位图):丢掉BMP文件的文件信息头和位图信息头，获取其RGB(A)位图数据  
U8* GetBmpData(U8 *bitCountPerPix, U32 *width, U32 *height, const char* filename)  
{  
    FILE *pf = fopen(filename, "rb");  
    if(!pf)  
    {  
        printf("fopen failed : %s, %d\n", __FILE__, __LINE__);  
        return NULL;  
    }  
  
    BITMAPFILE bmpfile;
    fread(&(bmpfile.bfHeader.bfType), sizeof(bmpfile.bfHeader.bfType), 1, pf);
    fread(&(bmpfile.bfHeader.bfSize), sizeof(bmpfile.bfHeader.bfSize), 1, pf);
    fread(&(bmpfile.bfHeader.bfReserved1), sizeof(bmpfile.bfHeader.bfReserved1), 1, pf);
    fread(&(bmpfile.bfHeader.bfReserved2), sizeof(bmpfile.bfHeader.bfReserved2), 1, pf);
    fread(&(bmpfile.bfHeader.bfOffBits), sizeof(bmpfile.bfHeader.bfOffBits), 1, pf);
    //if(0 > fread(&(bmpfile.bfHeader), sizeof(bmpfile.bfHeader), 1, pf))
    //{
    //    ERROR_MSG("E:fread bfheader error!\r\n");
    //}    
    printf("file header: type=0x%04x, size=%d, offbits=%d\r\n", \
           bmpfile.bfHeader.bfType, \
           bmpfile.bfHeader.bfSize, \
           bmpfile.bfHeader.bfOffBits);
     
    if(0 > fread(&(bmpfile.biInfo.bmiHeader), sizeof(bmpfile.biInfo.bmiHeader), 1, pf))
    {
        ERROR_MSG("E:fread bmiHeader error!\r\n");
    }
    printf("bmp info header: size=%d, width=%d, height=%d, bitcount=%d, biCompression=%d, biSizeImage=%d, biXPelsPerMeter=%d, biYPelsPerMeter=%d, biClrUsed=%d, biClrImportant=%d\r\n", \
           bmpfile.biInfo.bmiHeader.biSize, \
           bmpfile.biInfo.bmiHeader.biWidth, \
           bmpfile.biInfo.bmiHeader.biHeight, \
           bmpfile.biInfo.bmiHeader.biBitCount, \
           bmpfile.biInfo.bmiHeader.biCompression, \
           bmpfile.biInfo.bmiHeader.biSizeImage, \
           bmpfile.biInfo.bmiHeader.biXPelsPerMeter, \
           bmpfile.biInfo.bmiHeader.biYPelsPerMeter, \
           bmpfile.biInfo.bmiHeader.biClrUsed, \
           bmpfile.biInfo.bmiHeader.biClrImportant);
       
    *bitCountPerPix = bmpfile.biInfo.bmiHeader.biBitCount;  
    if(width)  
    {  
        *width = bmpfile.biInfo.bmiHeader.biWidth;  
    }  
    if(height)  
    {  
        *height = bmpfile.biInfo.bmiHeader.biHeight;  
    }  
  
    U32 bmppicth = (((*width)*(*bitCountPerPix) + 31) >> 5) << 2;
    printf("bmppicth=%d\r\n", bmppicth);
    U8 *pdata = (U8*)malloc((*height)*bmppicth);  
       
    U8 *pEachLinBuf = (U8*)malloc(bmppicth);  
    memset(pEachLinBuf, 0, bmppicth);  
    U8 BytePerPix = (*bitCountPerPix) >> 3;  
    U32 pitch = (*width) * BytePerPix;  
  
    if(pdata && pEachLinBuf)  
    {  
#if 0
        int w, h;  
        for(h = (*height) - 1; h >= 0; h--)  
        {  
            fread(pEachLinBuf, bmppicth, 1, pf);  
            for(w = 0; w < (*width); w++)  
            {  
                pdata[h*pitch + w*BytePerPix + 0] = pEachLinBuf[w*BytePerPix+0];  
                pdata[h*pitch + w*BytePerPix + 1] = pEachLinBuf[w*BytePerPix+1];  
                pdata[h*pitch + w*BytePerPix + 2] = pEachLinBuf[w*BytePerPix+2];  
            }  
        }  
#endif
        fseek(pf, bmpfile.bfHeader.bfOffBits, SEEK_SET);
        fread(pdata, (*height)*bmppicth, 1, pf); 
        
        free(pEachLinBuf);  
    }  
    fclose(pf);  
       
    return pdata;  
}  
  
//释放GetBmpData分配的空间  
void FreeBmpData(U8 *pdata)  
{  
    if(pdata)  
    {  
        free(pdata);  
        pdata = NULL;  
    }  
}  
  

