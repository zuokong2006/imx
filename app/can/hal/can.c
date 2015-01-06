/* name: can.c
 * description: CAN驱动接口
 * author: zuokongxiao
 * date: 2014-08-21
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
#include "deftype.h"
#include "debug.h"
#include "can.h"

#ifndef AF_CAN
#define AF_CAN 29
#endif
#ifndef PF_CAN
#define PF_CAN AF_CAN
#endif

/* can描述符 */
static int canfd = 0;

/* 打印CAN帧 */
static void print_frame(struct can_frame *fr)
{
    int i;
    char tmp[32] = { 0 }, buf[32] = { 0 };

    for(i = 0; i < fr->can_dlc; i++)
    {
        sprintf(tmp, "%02x ", fr->data[i]);
        strcat(buf, tmp);
    }
    DEBUG_MSG("ID=0x%08x, DLC=%d, DATA=%s\r\n", \
        fr->can_id & CAN_EFF_MASK, fr->can_dlc, buf);
}

#define errout(_s)	fprintf(stderr, "error class: %s\n", (_s))
#define errcode(_d) fprintf(stderr, "error code: %02x\n", (_d))
static void handle_err_frame(const struct can_frame *fr)
{
    if(fr->can_id & CAN_ERR_TX_TIMEOUT) {
        errout("CAN_ERR_TX_TIMEOUT");
    }
    if(fr->can_id & CAN_ERR_LOSTARB) {
        errout("CAN_ERR_LOSTARB");
        errcode(fr->data[0]);
    }
    if(fr->can_id & CAN_ERR_CRTL) {
        errout("CAN_ERR_CRTL");
        errcode(fr->data[1]);
    }
    if(fr->can_id & CAN_ERR_PROT) {
        errout("CAN_ERR_PROT");
        errcode(fr->data[2]);
        errcode(fr->data[3]);
    }
    if(fr->can_id & CAN_ERR_TRX) {
        errout("CAN_ERR_TRX");
        errcode(fr->data[4]);
    }
    if(fr->can_id & CAN_ERR_ACK) {
        errout("CAN_ERR_ACK");
    }
    if(fr->can_id & CAN_ERR_BUSOFF) {
        errout("CAN_ERR_BUSOFF");
    }
    if(fr->can_id & CAN_ERR_BUSERROR) {
        errout("CAN_ERR_BUSERROR");
    }
    if(fr->can_id & CAN_ERR_RESTARTED) {
        errout("CAN_ERR_RESTARTED");
    }
}


/* 打开CAN */
int can_open(void)
{
    int ret;
    struct sockaddr_can addr;
    struct ifreq ifr;

    canfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(canfd < 0)
    {
        ERROR_MSG("E:socket PF_CAN failed!\r\n");
        return -1;
    }
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(canfd, SIOCGIFINDEX, &ifr);
    if(ret < 0)
    {
        ERROR_MSG("E:ioctl failed!\r\n");
        return -1;
    }
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(canfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0)
    {
        ERROR_MSG("E:bind failed!\r\n");
        return -1;
    }
    /* CAN过滤器设置 
     * kernel\Documentation\networking\can.txt
     * <received_can_id> & mask == can_id & mask */
    struct can_filter filter[3];
    filter[0].can_id = (1<<22) | (15<<16) | CAN_EFF_FLAG;
    filter[0].can_mask = (1<<22) | (15<<16);
    filter[1].can_id = (2<<22) | (15<<16) | CAN_EFF_FLAG;
    filter[1].can_mask = (2<<22) | (15<<16);
    filter[2].can_id = (3<<22) | (15<<16) | CAN_EFF_FLAG;
    filter[2].can_mask = (3<<22) | (15<<16);
    //filter[3].can_id = 0x00010000 | CAN_EFF_FLAG;
    //filter[3].can_mask = 0x00010000;
    ret = setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
    /* 禁用接收过滤规则 */
    //ret = setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    if(ret < 0)
    {
        ERROR_MSG("E:setsockopt failed!\r\n");
        return -1;
    }
    
    return 0;
}

int can_close(void)
{
    close(canfd);
    return 0;
}

/* 阻塞读CAN */
int can_read(struct can_frame *pstCanFrame)
{
    int ret = 0;
    
    if((canfd <= 0) || INVALID_POINTER(pstCanFrame))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    ret = read(canfd, pstCanFrame, sizeof(*pstCanFrame));
    if(ret < sizeof(*pstCanFrame))
    {
        ERROR_MSG("E:read failed!\r\n");
        return -1;
    }
    /* 判断设备是否错误 */
    if(pstCanFrame->can_id & CAN_ERR_FLAG)
    { 
        handle_err_frame(pstCanFrame);
        DEBUG_MSG("E:CAN device error!\r\n");
        return -1;
    }
    /* 打印CAN帧 */
    print_frame(pstCanFrame);

    return 0;
}

/* can发送 */
int can_write(struct can_frame *pstCanFrame)
{
    if((canfd <= 0) || INVALID_POINTER(pstCanFrame))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    if(0 > write(canfd, pstCanFrame, sizeof(*pstCanFrame)))
    {
        DEBUG_MSG("E: can write failed!\r\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

/* can发送测试 */
int can_write_test(uint8 ucBoardType)
{
    struct can_frame stCanFrame;

    /* aa 00 03 01 00 01 05 55 */
    /* CAN ID 设置 */
    if((0 < ucBoardType) && (64 >= ucBoardType))
    {
        stCanFrame.can_id = 0x80000000 | (ucBoardType<<16);
    }
    else
    {
        stCanFrame.can_id = 0x80010000;
    }
    stCanFrame.can_dlc = 8;
    stCanFrame.data[0] = 0xaa;
    stCanFrame.data[1] = 0x00;
    stCanFrame.data[2] = 0x03;
    stCanFrame.data[3] = 0x01;
    stCanFrame.data[4] = 0x00;
    stCanFrame.data[5] = 0x01;
    stCanFrame.data[6] = 0x05;
    stCanFrame.data[7] = 0x55;
    return can_write(&stCanFrame);
}

/* 发送数据到CAN */
int send_data_to_can(int lBoardType, char *pcCmd, int lCmdLen)
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
        stCanFrame.can_id = 0x80000000 | (15 << 22) | (lBoardType << 16) |FRAME_NONE_SEG;
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
        stCanFrame.can_id = 0x80000000 | (15 << 22) | (lBoardType << 16) | FRAME_FIRST_SEG;
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
            stCanFrame.can_id = 0x80000000 | (15 << 22) | (lBoardType << 16) | FRAME_MIDDLE_SEG | (ucCnt << 8);
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
        stCanFrame.can_id = 0x80000000 | (15 << 22) | (lBoardType << 16) | FRAME_END_SEG | (ucCnt << 8);
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
