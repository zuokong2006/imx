/* name: cancmd.c
 * description: CAN命令程序
 * author: zuokongxiao
 * date: 2014-08-27
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
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <pthread.h>
#include <semaphore.h>
#include "deftype.h"
#include "debug.h"
#include "can.h"
#include "canrecthread.h"

/* 命令字符串转换 */
static int cmd_string_convert(char *pcCmd, const char *pcCmdBuf)
{
    int ret = 0;
    int i = 0, cmdi = 0, hib = 0, lob = 0;

    if(INVALID_POINTER(pcCmdBuf) || INVALID_POINTER(pcCmd))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    for(i=0; i<1021; i=i+3)
    {
        if('\0' == pcCmdBuf[i])
        {
            ret = cmdi;
            break;
        }
        /* 高位 */
        if(('0' <= pcCmdBuf[i]) && ('9' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - '0';
        }
        else if(('a' <= pcCmdBuf[i]) && ('f' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - 'a' + 10;
        }
        else if(('A' <= pcCmdBuf[i]) && ('F' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - 'A' + 10;
        }
        else
        {
            DEBUG_MSG("E:high 4 bits error!\r\n");
            ret = -1;
            break;
        }
        /* 低位 */
        if(('0' <= pcCmdBuf[i+1]) && ('9' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - '0';
        }
        else if(('a' <= pcCmdBuf[i+1]) && ('f' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - 'a' + 10;
        }
        else if(('A' <= pcCmdBuf[i+1]) && ('F' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - 'A' + 10;
        }
        else
        {
            DEBUG_MSG("E:low 4 bits error!\r\n");
            ret = -1;
            break;
        }
        hib *= 16;
        hib = hib + lob;
        pcCmd[cmdi] = (char)hib;
        cmdi++;
        if('\0' == pcCmdBuf[i+2])
        {
            ret = cmdi;
            break;
        }
        if((' ' != pcCmdBuf[i+2]) && ('\0' != pcCmdBuf[i+2]))
        {
            DEBUG_MSG("E:space error! i=%d cmdi=%d\r\n", i, cmdi);
            ret = -1;
            break;
        }
    }
    
    return ret;
}

/* 发送数据到CAN */
static int send_data_to_can(int lBoardType, char *pcCmd, int lCmdLen)
{
    struct can_frame stCanFrame;
    int i = 0;
    uint8 ucCnt = 0;
    uint16 unTemp = 0;
    
    if(INVALID_POINTER(pcCmd) || (1 > lCmdLen) || (1 > lBoardType))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /*! 计算数据需要几个报文帧 */
    unTemp = lCmdLen / 8;
    if(0x0000 != (lCmdLen & 0x0007))
    {
        unTemp = unTemp + 1;
    }
    /*! 数据不大于8字节，无需分段 */
    if(1 == unTemp)
    {
        stCanFrame.can_id = 0x80000000 | (lBoardType << 16) |FRAME_NONE_SEG;
        stCanFrame.can_dlc = lCmdLen;
        for(i = 0; i < lCmdLen; i++)
        {
            stCanFrame.data[i] = pcCmd[i];
        }
        if(0 > can_write(&stCanFrame))
        {
            return -1;
        }
    }
    else
    {
        /*! 第一个分段报文 */
        stCanFrame.can_id = 0x80000000 | (lBoardType << 16) | FRAME_FIRST_SEG;
        stCanFrame.can_dlc = 8;
        for(i = 0; i<8; i++)
        {
            stCanFrame.data[i] = *pcCmd;
            pcCmd++;
        }
        if(0 > can_write(&stCanFrame))
        {
            return -1;
        }
        unTemp--;
        ucCnt = 1;
        /*! 中间分段报文 */
        while(1 != unTemp)
        {
            stCanFrame.can_id = 0x80000000 | (lBoardType << 16) | FRAME_MIDDLE_SEG | (ucCnt << 8);
            stCanFrame.can_dlc = 8;
            for(i = 0; i<8; i++)
            {
                stCanFrame.data[i] = *pcCmd;
                pcCmd++;
            }
            if(0 > can_write(&stCanFrame))
            {
                return -1;
            }
            unTemp--;
            ucCnt++;
        }
        /*! 最后分段报文 */
        if(0x0000 == (lCmdLen & 0x0007))
        {
            unTemp = 8;
        }
        else
        {
            unTemp = lCmdLen & 0x0007;
        }
        ucCnt = 0;
        stCanFrame.can_id = 0x80000000 | (lBoardType << 16) | FRAME_END_SEG | (ucCnt << 8);
        stCanFrame.can_dlc = unTemp;
        for(i = 0; i<unTemp; i++)
        {
            stCanFrame.data[i] = *pcCmd;
            pcCmd++;
        }
        if(0 > can_write(&stCanFrame))
        {
            return -1;
        }
    }

    return 0;
}

/* 主函数 */
int main(int argc, char *argv[])
{
    int lBoardType = 0, i = 0, lCmdLen = 0;
    char acCmdBuf[1024] = {0};
    char acCmd[340] = {0};
    pthread_t thread_pid = 0;
    
    /* 打开CAN设备 */
    if(0 > can_open())
    {
        return -1;
    }
    /* 创建接收线程 */
    if(0 != pthread_create(&thread_pid, NULL, can_rec_thread, NULL))
    {
        ERROR_MSG("E:thread create failed!\r\n");
        return -1;
    }
    DEBUG_MSG("D:thread_create is ok!\r\n");
   
    while(1)
    {
        /* 输入板卡类型 */
        //printf("\r\n***************************\r\n");
        printf("Please input board type: ");
        scanf("%d", &lBoardType);
        printf("board type = %d\r\n", lBoardType);
        if((0 > lBoardType) || (3 < lBoardType))
        {
            DEBUG_MSG("E:input boardtype < 0 or boardtype >3!\r\n");
            continue;
        }
        /* 输入命令字符串 */
        setbuf(stdin, NULL);
        printf("Please input cmd: ");
        //gets(acCmdBuf);
        for(i=0; i<1023; i++)
        {
            acCmdBuf[i] = getchar();
            if((0x00 == acCmdBuf[i]) || ('\n' == acCmdBuf[i]))
            {
                acCmdBuf[i] = '\0';
                break;
            }
        }
        acCmdBuf[i+1] = '\0';
        printf("%s\r\n", acCmdBuf);
        
        /* 输入字符串转换 */
        lCmdLen = cmd_string_convert(acCmd, acCmdBuf);
        if(0 >= lCmdLen)
        {
            continue;
        }
        printf("cmd: ", acCmdBuf);
        for(i=0; i<lCmdLen; i++)
        {
            printf("0x%02x ", acCmd[i]);
        }
        printf("\r\n");
        
        /* cmd格式检查 */
        //todo...
        
        /* can发送数据 */
        send_data_to_can(lBoardType, acCmd, lCmdLen);
    }
}



