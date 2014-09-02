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
#define REC_BUF_NUM         4u

/* 报文段缓冲区结构体定义 */
struct seg_msg_buf
{
    uint8 aucSegMsgBuf[PROTOCOL_FRAME_MAX_SIZE*2]; /* 帧缓冲区（分段报文重组缓冲区） */
    uint8 ucSegMsgCnt; /* 帧个数 */
    uint8 ucLastSegNum; /* 上一次分段号 */
};

/* 报文段重组 */
static int can_seg_msg_recombine(frame_check_t *pstFrame, \
                                 struct seg_msg_buf *pstSegMsg, \
                                 struct can_frame *pstCanFrame)
{
    int ret = -1;
    int i = 0;
    uint8 ucSegNum = 0;
    
    do
    {
        if(INVALID_POINTER(pstFrame) || INVALID_POINTER(pstSegMsg) \
           || INVALID_POINTER(pstCanFrame))
        {
            DEBUG_MSG("E:input param error!\r\n");
            break;
        }
        /* 获取协议分段号 */
        ucSegNum = (uint8)((pstCanFrame->can_id & SEG_CODE_NUM_MASK)>>8);
        
        /*! 协议帧无分段报文 */
        if(FRAME_NONE_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            /*! 直接将数据复制到CAN接收缓冲区 */
            for(i=0; i<pstCanFrame->can_dlc; i++)
            {
                pstFrame->stRecBuf.aucBuf[pstFrame->stRecBuf.unWrIndex] \
                    = pstCanFrame->data[i];
                (pstFrame->stRecBuf.unWrIndex)++;
                if(REC_BUF_SIZE <= pstFrame->stRecBuf.unWrIndex)
                {
                    pstFrame->stRecBuf.unWrIndex = 0;
                }
            }
        }
        /*! 协议帧第一个分段报文（分段报文必须依次发送） */
        else if(FRAME_FIRST_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            if((0 != ucSegNum) || (8 != pstCanFrame->can_dlc))
            {
                DEBUG_MSG("E: ucSegNum != 0 or can_dlc != 8 !\r\n");
                break;
            }
            /*! 将报文数据复制到协议帧缓冲区 */
            pstSegMsg->ucSegMsgCnt = 0;
            for(i=0; i<8; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            pstSegMsg->ucLastSegNum = ucSegNum;
        }
        /*! 协议帧中间分段报文 */
        else if(FRAME_MIDDLE_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            (pstSegMsg->ucLastSegNum)++;
            if((pstSegMsg->ucLastSegNum != ucSegNum) \
                || (8 != pstCanFrame->can_dlc) \
                || (1 > ucSegNum) || (48 < ucSegNum))
            {
                DEBUG_MSG("E: SegNum != LastSegNum or can_dlc != 8 or ucSegNum value is invalid!\r\n");
                pstSegMsg->ucLastSegNum = 0xff; /*!< 中间分段报文格式错误 */
                break;
            }
            /*! 将报文数据复制到协议帧缓冲区 */
            for(i=0; i<8; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            pstSegMsg->ucLastSegNum = ucSegNum;
        }
        /*! 协议帧的最后分段报文 */
        else if(FRAME_END_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            if((0 != ucSegNum) || (0xff == pstSegMsg->ucLastSegNum))
            {
                DEBUG_MSG("E: SegNum != 0 or ucLastSegNum value is invalid(0xff)!\r\n");
                break;
            }
            /*! 将报文数据复制到协议帧缓冲区 */
            for(i=0; i<pstCanFrame->can_dlc; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            /*! 接收到最后分段报文后，将协议帧缓冲区的数据复制到CAN接收缓冲区 */
            for(i=0; i<(pstSegMsg->ucSegMsgCnt); i++)
            {
                pstFrame->stRecBuf.aucBuf[pstFrame->stRecBuf.unWrIndex] \
                    = pstSegMsg->aucSegMsgBuf[i];
                (pstFrame->stRecBuf.unWrIndex)++;
                if(REC_BUF_SIZE <= pstFrame->stRecBuf.unWrIndex)
                {
                    pstFrame->stRecBuf.unWrIndex = 0;
                }
            }
        }
        else
        {
            break;
        }
        ret = 0;
    }while(0);
    
    return ret;
}

/* can接收线程 */
void *can_rec_thread(void *arg)
{
    struct can_frame stCanFrame;
    uint8 ucSrcId = 0, ucDestId = 0;
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
        if((3 < ucSrcId) || (3 < ucDestId))
        {
            DEBUG_MSG("E:srcid > 3 or destid > 3! srcid=%d desti=%d\r\n", \
                      ucSrcId, ucDestId);
            continue;
        }

        /* 报文段重组 */
        if(0 > can_seg_msg_recombine(&(astFrameBuf[ucSrcId]), \
                                     &(astSegMsgBuf[ucSrcId]), &stCanFrame))
        {
            DEBUG_MSG("E:can_seg_msg_recombine() return error!\r\n");
            continue;
        }
        
        /* 协议解析 */
        if(EXIT_FAILURE == protocol_process(ucSrcId, &(astFrameBuf[ucSrcId])))
        {
            DEBUG_MSG("E:Protocol process error!\r\n");
            continue;
        }
        
        /* 判断链表中是否有等待的对象 */
        /* 线程锁 */
        /* 有，释放信号量 */
        
    } /* end of while(1)... */
}





