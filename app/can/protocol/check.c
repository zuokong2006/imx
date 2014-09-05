/*! \file  check.c
* \b 接口说明：
*
*       1. check_protocol_frame: 检查协议帧格式 <br>
*
* \b 功能描述:
*
*       帧格式检查
*
* \b 历史记录:
*
*     <作者>        <时间>      <修改记录> <br>
*     zuokongxiao  2013-03-13   创建文件   <br>
*/

/******************************************************************************/
/*                             头文件                                         */
/******************************************************************************/
#include "deftype.h"
#include "protocolio.h"
#include "protocol.h"
#include "debug.h"
//#include "led.h"
#include "identify.h"
//#include "command.h"

/******************************************************************************/
/*                             宏定义                                         */
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
/*! \fn       int32 check_frame_start(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     帧起始码判断
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_start(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_FAILURE;
    
    if(PROTOCOL_FRAME_START_CODE == ucFrameData)
    {
        pstFrame->ucCheckState = FRAME_CHECK_CONTROL;
        pstFrame->ucFrameBufIndex = 0;
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        lRet = EXIT_SUCCESS;
    }
    else
    {
        pstFrame->ucCheckState = FRAME_CHECK_ERROR;
        *punErrCode = 0x0001;
    }
    
    return lRet;
}

/*! \fn       int32 check_frame_control(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     帧控制码判断
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_control(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_FAILURE;
    
    switch(ucFrameData)
    {
        /*! 控制码 */
        case FRAME_CONTROL_CODE_COMMAND_REP:
        case FRAME_CONTROL_CODE_COMMAND_NO_REP:
        case FRAME_CONTROL_CODE_TS_HEAD:
        case FRAME_CONTROL_CODE_TS_BODY:
        case FRAME_CONTROL_CODE_TS_TAIL:
        case FRAME_CONTROL_CODE_REP:
        case FRAME_CONTROL_CODE_ACK:
        case FRAME_CONTROL_CODE_EVT:
            pstFrame->ucCheckState = FRAME_CHECK_DATALEN;
            pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
            /*! 校验和初值 */
            pstFrame->ucCheckSum = ucFrameData;
            pstFrame->ucFrameBufIndex++;
            lRet = EXIT_SUCCESS;
            break;
        /*! 错误控制码 */
        default:
            pstFrame->ucCheckState = FRAME_CHECK_ERROR;
            *punErrCode = 0x0002;
            break;
    }
    
    return lRet;
}

/*! \fn       int32 check_frame_datalen(uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     帧数据长度判断
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_datalen(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_FAILURE;
    
    /*! 帧始终有3字节的命令码 */
    if((3 <= ucFrameData) && ((PROTOCOL_FRAME_MAX_SIZE-5) >= ucFrameData))
    {
        pstFrame->ucCheckState = FRAME_CHECK_DATA;
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        /*! 保存长度值 */
        pstFrame->ucFrameDataLen = ucFrameData;
        /*! 计算校验和 */
        pstFrame->ucCheckSum += ucFrameData;
        lRet = EXIT_SUCCESS;
    }
    else
    {
        pstFrame->ucCheckState = FRAME_CHECK_ERROR;
        *punErrCode = 0x0003;
    }
    
    return lRet;
}

/*! \fn       int32 check_frame_data(uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     帧数据
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_data(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_SUCCESS;
    
    if(0 < pstFrame->ucFrameDataLen)
    {
        pstFrame->ucFrameDataLen--;
        /*! 保存数据 */
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        /*! 计算校验和 */
        pstFrame->ucCheckSum += ucFrameData;
    }
    /*! 判断数据是否接收完成 */
    if(0 == pstFrame->ucFrameDataLen)
    {
        pstFrame->ucCheckState = FRAME_CHECK_CHECKSUM;
    }
    
    return lRet;
}

/*! \fn       int32 check_frame_checksum(uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     检查帧校验和
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_checksum(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_FAILURE;
    
    if(pstFrame->ucCheckSum == ucFrameData)
    {
        pstFrame->ucCheckState = FRAME_CHECK_STOP;
        /*! 保存数据 */
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        lRet = EXIT_SUCCESS;
    }
    else
    {
        /*! 校验和错误 */
        pstFrame->ucCheckState = FRAME_CHECK_ERROR;
        *punErrCode = 0x0004;
    }
    
    return lRet;
}

/*! \fn       int32 check_frame_stop(uint8 ucFrameData, uint16 *punErrCode)
*
*  \brief     检查帧停止码
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_frame_stop(frame_check_t *pstFrame, uint8 ucFrameData, uint16 *punErrCode)
{
    int32 lRet = EXIT_FAILURE;
    
    if(PROTOCOL_FRAME_STOP_CODE == ucFrameData)
    {
        pstFrame->ucCheckState = FRAME_CHECK_START;
        /*! 保存数据 */
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        lRet = EXIT_SUCCESS;
    }
    else
    {
        /*! 校验和错误 */
        pstFrame->ucCheckState = FRAME_CHECK_ERROR;
        *punErrCode = 0x0005;
    }
    
    return lRet;
}

/*! \fn       int32 self_check_frame(uint8 ucFrameData)
*
*  \brief     自检
*
*  \param     ucFrameData [in]  帧数据
*
*  \param     punErrCode  [out] 错误码
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 self_check_frame(frame_check_t *pstFrame, uint8 ucFrameData)
{
    int32 lRet = EXIT_FAILURE;
    
    /*! 检测到0x55 0xAA组合，则认为检测到新的通讯帧 */
    if((PROTOCOL_FRAME_STOP_CODE == pstFrame->ucPreFrameData) \
        && (PROTOCOL_FRAME_START_CODE == ucFrameData))
    {
        pstFrame->ucCheckState = FRAME_CHECK_CONTROL;
        /*! 保存数据 */
        pstFrame->ucFrameBufIndex = 0;
        pstFrame->aucFrameBuf[pstFrame->ucFrameBufIndex] = ucFrameData;
        pstFrame->ucFrameBufIndex++;
        lRet = EXIT_SUCCESS;
    }
    else
    {
        pstFrame->ucPreFrameData = ucFrameData;
    }
    
    return lRet;
}


/******************************************************************************/
/*                                外部接口                                    */
/******************************************************************************/
/*! \fn       int32 check_protocol_frame(uint8 ucNum, frame_check_t *pstFrame)
*
*  \brief     协议帧格式检查
*
*  \param     ucNum    [in] 帧BUF
*
*  \param     pstFrame [in] 帧缓冲区
*
*  \exception 无
*
*  \return    EXIT_FAILURE：失败；EXIT_SUCCESS：成功
*/
int32 check_protocol_frame(uint8 ucNum, frame_check_t *pstFrame)
{
    int32 lRet = EXIT_FAILURE;
    uint8 ucRecData = 0;
    uint8 *pucRecBuf = NULL;
    uint16 *punRecBufWriteIndex = NULL;
    uint16 *punRecBufReadIndex = NULL;
    uint16 unErrCode = 0;
    
    do
    {
        /*! 获取接收缓冲区的写索引 */
        punRecBufWriteIndex = &(pstFrame->stRecBuf.unWrIndex);
        if(INVALID_POINTER(punRecBufWriteIndex))
        {
            DEBUG_MSG("E:get write index error!\r\n");
            break;
        }
        /*! 获取接收缓冲区的读索引 */
        punRecBufReadIndex = &(pstFrame->stRecBuf.unRdIndex);
        if(INVALID_POINTER(punRecBufReadIndex))
        {
            DEBUG_MSG("E:get read index error!\r\n");
            break;
        }
        /*! 判断接收缓冲区是否有数据 */
        if((*punRecBufWriteIndex) == (*punRecBufReadIndex))
        {
            lRet = EXIT_SUCCESS;
            break;
        }
        /*! 获取接收缓冲区 */
        pucRecBuf = pstFrame->stRecBuf.aucBuf;
        if(INVALID_POINTER(pucRecBuf))
        {
            DEBUG_MSG("E:get recbuf addr error!\r\n");
            break;
        }
        /*! 检查缓冲区 */
        lRet = EXIT_SUCCESS;
        while((*punRecBufWriteIndex) != (*punRecBufReadIndex))
        {
            /*! 获取缓冲器的数据 */
            ucRecData = pucRecBuf[(*punRecBufReadIndex)];
            (*punRecBufReadIndex)++;
            if(REC_BUF_SIZE <= (*punRecBufReadIndex))
            {
                *punRecBufReadIndex = 0;
            }
            /*! 检查获取的数据格式 */
            switch(pstFrame->ucCheckState)
            {
                /*! 起始码 */
                case FRAME_CHECK_START:
                    pstFrame->stReturnBuf.ucFlag = 0;
                    check_frame_start(pstFrame, ucRecData, &unErrCode);
                    break;
                /*! 控制码 */
                case FRAME_CHECK_CONTROL:
                    check_frame_control(pstFrame, ucRecData, &unErrCode);
                    break;
                /*! 数据长度 */
                case FRAME_CHECK_DATALEN:
                    check_frame_datalen(pstFrame, ucRecData, &unErrCode);
                    break;
                /*! 数据 */
                case FRAME_CHECK_DATA:
                    check_frame_data(pstFrame, ucRecData, &unErrCode);
                    break;
                /*! 校验和 */
                case FRAME_CHECK_CHECKSUM:
                    check_frame_checksum(pstFrame, ucRecData, &unErrCode);
                    break;
                /*! 结束码*/
                case FRAME_CHECK_STOP:
                    if(EXIT_SUCCESS == check_frame_stop(pstFrame, ucRecData, &unErrCode))
                    {
                        //debug_msg("check frame ok!", __FILE__, __LINE__);
                        //LED_STATE = ~LED_STATE;
                        /*! 识别协议帧 */
                        lRet = identify_protocol_frame(ucNum, pstFrame->aucFrameBuf, &(pstFrame->stReturnBuf));
                        pstFrame->stReturnBuf.ucFlag = 1;
                    }
                    break;
                
                /*! 帧检查错误 */
                case FRAME_CHECK_ERROR:
                    if(EXIT_SUCCESS == self_check_frame(pstFrame, ucRecData))
                    {
                        DEBUG_MSG("D:self-check ok!\r\n");
                        pstFrame->stReturnBuf.ucFlag = 0;
                    }
                    break;
                /*! 其他值 */
                default:
                    break;
            }/*!< end switch(s_stCheckFrame.ucCheckState)... */
            
            /*! 判断检查帧格式是否有错误 */
            if(0x00000 != unErrCode)
            {
                /*! 通信故障，返回EVT */
                pstFrame->stReturnBuf.ucFlag = NEED_EVT;
                pstFrame->stReturnBuf.ucControlCode = FRAME_CONTROL_CODE_EVT;
                pstFrame->stReturnBuf.ucCmdType = 0x00;
                pstFrame->stReturnBuf.ucCmdNum = 0x00;
                pstFrame->stReturnBuf.ucCmdContent = 0x0006;
                pstFrame->stReturnBuf.unReserve = 0x0000;
                pstFrame->stReturnBuf.unErrCode = unErrCode;
                pstFrame->stReturnBuf.ucStateLen = 0;
                pstFrame->stReturnBuf.pucStateBuf = NULL;
                //lRet = send_return_frame(&s_stReturnBuf);
                /*! 打印错误码 */
                //put_hex(unErrCode, NULL);
                DEBUG_MSG("E:check frame error! errcode=0x%04x\r\n", unErrCode);
                unErrCode = 0;
            }
        } /*!< end while((*punRecBufWriteIndex) != (*punRecBufReadIndex))... */
    }while(0);
    
    return lRet;
}

/********************************end of file***********************************/

