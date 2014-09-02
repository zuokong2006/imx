#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "librf.h"

int main(int argc, char *argv[])
{
    int i,st;
    static unsigned long snr;
    static unsigned char data[16];
    static unsigned char buff[16];
    int par=2;
    long baud=115200;
    int icdev;
    unsigned long rdata;
    unsigned int tagtype;
    unsigned long cardsnr = 0;
    unsigned char strSize[5];
    unsigned char tk[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    
    for(i=0;i<16;i++)
    {
        buff[i]=0xff;
        data[i]=0x81;
    }
    /* 初始化通讯口：100表示USB口通讯，此时波特率无效 */
    icdev=dc_init(100,baud);
    if(icdev<0)
    {
        printf("rf_init error.");
        exit(1);
    }
    /* 蜂鸣器响200ms */
    if(dc_beep(icdev, 20)!=0)
    {
        printf("dc_beep error.");
        exit(1);
    }
    /* 读取硬件版本号 */
    if(dc_getver(icdev, strSize)!=0)
    {
        printf("dc_getver error.");
        exit(1);
    }
    printf(strSize); /* 打印版本号 */
    printf("\n");
    
    /* 将密码装入读写模块RAM中 */
    if(dc_load_key(icdev,0,0,tk)!=0)
    {
        printf("\nload_key (A) error.\n");
        dc_exit(icdev);
        exit(1);
    }
  
    do
    {
        printf("\nNow you can place your card in the fileds or the antenna...\nPress ESC key to quit...\n\n");
        if((dc_reset(icdev,0))!=0) {printf("dc_reset error!\n"); continue;}
        cardsnr=0;
#if 0
        /* 寻卡，能返回在工作区域内某张卡的序列号(该函数包含了dc_request,dc_anticoll,dc_select的整体功能) */
        if((dc_card(icdev,0,&snr))!=0) {printf("dc_card error!\n"); continue;}
#else
        /* 寻卡请求 */
        if( dc_request(icdev,0,&tagtype)!=0) {printf("dc_request error!\n"); continue;}
        /* 防卡冲突，返回卡的序列号 */
        if(dc_anticoll(icdev,0,&cardsnr)!=0){printf("dc_anticoll error!\n"); continue;}
        /* 从多个卡中选取一个给定序列号的卡 */
        if(dc_select(icdev,cardsnr,strSize)!=0){printf("dc_select error!\n"); continue;}
#endif
        /* 核对密码函数 */
        if((dc_authentication(icdev,0,1))!=0) {printf("dc_authentication error!\n"); continue;}
        /* 向卡的块4写入数据 */
        if(dc_write(icdev, 4, data)!=0) {printf("dc_write error!\n"); continue;}
        /* 读取数据 */
        if((dc_read(icdev, 4, data))!=0) {printf("dc_read error!\n"); continue;}
        printf("serial number: %08lX\n",cardsnr);
        
        for(i=0;i<16;i++)
            printf("%02X ",data[i]);
        printf("\n");
        if(dc_write(icdev,4,buff)!=0)continue;
        printf("\nTEST CARD OK!\n");
   
        if(dc_initval(icdev,4,1000)!=0) continue;
        printf("\n dc_initval OK!\n");
        if(dc_increment(icdev,4,1)!=0) continue;
        printf("\n dc_increment OK!\n");
        if(dc_decrement(icdev,4,1)!=0) continue;
        printf("\n dc_decrement OK!\n");
        if(st=(dc_halt(icdev))==0)
        {
            st=dc_beep(icdev,1);
        }
        else
        {
            printf("dc_halt 0x%02x\n",st);
        }
        if(dc_halt(icdev)!=0)
        {
            printf("\n dc_halt error.\n");
            //dc_exit(icdev);
            //return -1;
        }
    }while(getchar()!=27);
    
    dc_exit(icdev);
    return 0;
}
