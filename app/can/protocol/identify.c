/*! \file  identify.c
* \b 接口说明：
*
*       1. identify_protocol_frame: 识别协议帧 <br>
*
* \b 功能描述:
*
*       协议帧识别
*
* \b 历史记录:
*
*     <作者>        <时间>      <修改记录> <br>
*     zuokongxiao  2013-03-14   创建文件   <br>
*     zuokongxiao  2013-10-15   时序包包体中增加命令号字段 <br>
*     zuokongxiao  2014-02-26   修改时序包为交替存放方式 <br>
*/

/******************************************************************************/
/*                             头文件                                         */
/******************************************************************************/
#include <stdio.h>
#include "deftype.h"
#include "protocol.h"
#include "debug.h"
//#include "explain.h""
//#include "command.h"

/******************************************************************************/
/*                              宏定义                                        */
/******************************************************************************/

/******************************************************************************/
/*                              结构定义                                      */
/******************************************************************************/

/******************************************************************************/
/*                             变量定义                                       */
/******************************************************************************/

/******************************************************************************/
/*                            内部接口声明                                    */
/******************************************************************************/


/******************************************************************************/
/*                            内部接口实现                                    */
/******************************************************************************/
/*! \fn       static int32 send_protocol_ack(int32 lFlag, return_buf_t *pstReturnBuf)
*
*  \brief     发送ACK
*
*  \param     lFlag        [in] lRet
*
*  \param     pstReturnBuf [in] 帧返回缓冲区
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
static int32 send_protocol_ack(int32 lFlag, return_buf_t *pstReturnBuf)
{
    if(EXIT_SUCCESS != lFlag)
    {
        /*! 出错返回EVT */
        pstReturnBuf->ucFlag = NEED_EVT;
        pstReturnBuf->ucControlCode = FRAME_CONTROL_CODE_EVT;
        if(0x0000 == pstReturnBuf->unErrCode)
        {
            /*! 其他错误：添加统一故障码（驱动板bug） */
            pstReturnBuf->unErrCode = 0x0010;
        }
        return send_return_frame(pstReturnBuf);
    }
    else
    {
#if CONFIG_PROTOCOL_ACK_EN > 0 /*!< 返回ACK */
        return send_return_frame(pstReturnBuf);
#else /*!< 不返回ACK */
        return EXIT_SUCCESS;
#endif
    }
}

/******************************************************************************/
/*                                外部接口                                    */
/******************************************************************************/
/*! \fn       int32 identify_protocol_frame(uint8 ucNum, const uint8 *pucFrameBuf, \
*                                           return_buf_t *pstReturnBuf)
*
*  \brief     识别协议帧
*
*  \param     ucNum        [in] 缓冲区号
*
*  \param     pucFrameBuf  [in] 指向需识别的协议帧
*
*  \param     pstReturnBuf [in] 帧返回缓冲区
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 identify_protocol_frame(uint8 ucNum, const uint8 *pucFrameBuf, \
                              return_buf_t *pstReturnBuf)
{
    int32 lRet = EXIT_FAILURE;
    uint8 ucIndentifyCode = 0;
    int32 i = 0;
    
    do
    {
        /*! 输入参数检查 */
        if(INVALID_POINTER(pucFrameBuf) || INVALID_POINTER(pstReturnBuf))
        {
            DEBUG_MSG("E:input param error!\r\n");
            break;
        }
        /*! 获取识别码 */
        ucIndentifyCode = pucFrameBuf[FRAME_CONTROL_INDEX] & CONTROL_CODE_IDENT_MASK;
        /*! 准备ACK数据 */
        pstReturnBuf->ucFlag = NEED_ACK;
        pstReturnBuf->ucControlCode = FRAME_CONTROL_CODE_ACK;
        pstReturnBuf->ucCmdType = pucFrameBuf[FRAME_COMMAND_TYPE_INDEX];
        pstReturnBuf->ucCmdNum = pucFrameBuf[FRAME_COMMAND_NUM_INDEX];;
        pstReturnBuf->ucCmdContent = pucFrameBuf[FRAME_COMMAND_CONTENT_INDEX];;
        pstReturnBuf->unReserve = 0x0000;
        pstReturnBuf->unErrCode = 0x0000;
        pstReturnBuf->ucStateLen = 0;
        pstReturnBuf->pucStateBuf = NULL;
        /*! 识别 */
        switch(ucIndentifyCode)
        {
            /*! 命令 */
            case CONTROL_CODE_IDENT_CMD:
            /*! 时序包包头 */
            case CONTROL_CODE_IDENT_TS_HEAD:
            /*! 时序包包体 */
            case CONTROL_CODE_IDENT_TS_BODY:
            /*! 时序包包尾 */
            case CONTROL_CODE_IDENT_TS_TAIL:
                //if(EXIT_SUCCESS == send_protocol_ack(EXIT_SUCCESS, pstReturnBuf))
                //{
                //    /*! 解释命令帧 */
                //    lRet = explain_cmd_frame(pucFrameBuf, pstReturnBuf);
                //}

                /* 打印返回数据 */
                DEBUG_MSG("D:board type=%d, data= ", ucNum);
                for(i=0; i<(pucFrameBuf[FRAME_DATALEN_INDEX]+5); i++)
                {
                    printf("0x%02x ", pucFrameBuf[i]);
                }
                printf("\r\n");
                
                break;

            /*! 其他值错误 */
            default:
                DEBUG_MSG("E:what's this?\r\n");
                break;
        }
    }while(0);
    
    return lRet;
}

/********************************end of file***********************************/

