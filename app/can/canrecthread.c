/* name: canrecthread.c
* description: CAN接收线程
* author: zuokongxiao
* date: 2014-08-28
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
#include "protocol.h"


/* 接收缓冲区数 */
#define REC_BUF_NUM         3u

struct seg_msg_buf
{
    uint8 aucSegMsgBuf[PROTOCOL_FRAME_MAX_SIZE*2]; /* 帧缓冲区（分段报文重组缓冲区） */
    uint8 ucSegMsgCnt; /* 帧个数 */
    uint8 ucLastSegNum; /* 上一次分段号 */
};

/* can接收线程 */
void *can_rec_thread(void *arg)
{
    struct can_frame stCanFrame;
    uint8 ucSrcId = 0, ucDestId = 0
    frame_check_t astFrameBuf[REC_BUF_NUM] = { 0 };
    struct seg_msg_buf astSegMsgBuf[REC_BUF_NUM] = { 0 };

    while(1)
    {
        /* 阻塞读取CAN帧 */
        if(0 > can_read(&stCanFrame))
        {
            DEBUG_MSG("E:can read error!\r\n");
            continue;
        }

        /*! 报文帧判断 */
        if((0x80000000 != (stCanFrame.can_id & 0x80000000)) \
            || (1 > stCanFrame.can_dlc))
        {
            DEBUG_MSG("E:can frame error(not data frame or dlc < 1)!\r\n");
            continue;
        }
        /*! 通过报文帧标识符获取源节点、目标节点 */
        ucSrcId = (uint8)((stCanFrame.can_id & SRC_ID_MASK) >> 22);
        ucDestId = (uint8)((stCanFrame.can_id & DEST_ID_MASK) >> 16);
        //ucSegNum = (uint8)((stCanFrame.can_id & SEG_CODE_NUM_MASK) >> 8);
        if((1 < ucSrcId) || ((3 < ucSrcId)) || (0 != ucDestId))
        {
            DEBUG_MSG("E:srcid != 0 or destid > 3 or destid == 0 !\r\n");
            continue;
        }


    }
}





