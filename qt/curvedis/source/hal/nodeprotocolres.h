
#ifndef __NODEPROTOCOLRES_H
#define __NODEPROTOCOLRES_H

/*! 协议帧最大长度*/
#define PROTOCOL_FRAME_MAX_SIZE                 (250u)
/*! ACK帧最大长度 */
#define ACK_FRAME_MAX_SIZE                      (10u)
/*! REP帧最大长度 */
#define REP_FRAME_MAX_SIZE                      (32u)
/*! EVT帧最大长度 */
#define EVT_FRAME_MAX_SIZE                      (10u)
/*! 查询命令返回数据的最大长度 */
#define QUERY_CMD_STATE_DATA_MAX_SIZE           (PROTOCOL_FRAME_MAX_SIZE-8)

/*! 协议帧起始码 */
#define PROTOCOL_FRAME_START_CODE               (0xAA)
/*! 协议帧停止码 */
#define PROTOCOL_FRAME_STOP_CODE                (0x55)

/*! 协议帧格式下标 */
#define FRAME_START_INDEX                       (0u)
#define FRAME_CONTROL_INDEX                     (1u)
#define FRAME_DATALEN_INDEX                     (2u)
#define FRAME_COMMAND_INDEX                     (3u)
#define FRAME_COMMAND_TYPE_INDEX                (3u)
#define FRAME_COMMAND_NUM_INDEX                 (4u)
#define FRAME_COMMAND_CONTENT_INDEX             (5u)
#define FRAME_COMMAND_PARAM_INDEX               (6u)

/*! 控制码编码 */
#define FRAME_CONTROL_CODE_COMMAND_REP          (0x00) /*!< 下传命令帧(REP) */
#define FRAME_CONTROL_CODE_COMMAND_NO_REP       (0x40) /*!< 下传命令帧(NO REP) */
#define FRAME_CONTROL_CODE_TS_HEAD              (0x41)
#define FRAME_CONTROL_CODE_TS_BODY              (0x42)
#define FRAME_CONTROL_CODE_TS_TAIL              (0x43)
#define FRAME_CONTROL_CODE_REP                  (0xC0) /*!< 从站响应帧 */
#define FRAME_CONTROL_CODE_ACK                  (0xD4) /*!< 从站应答帧 */
#define FRAME_CONTROL_CODE_EVT                  (0xF8) /*!< 从站异常帧 */
#define FRAME_CONTROL_CODE_UPLOAD               (0xD8) /*!< 从站主动上传帧 */

/*! 控制码mask */
#define CONTROL_CODE_TRANSDIR_MASK              (0x80)
#define CONTROL_CODE_RETURNFLAG_MASK            (0x40)
#define CONTROL_CODE_ABNORMALFLAG_MASK          (0x20)
#define CONTROL_CODE_FUNCTION_MASK              (0x1C)
#define CONTROL_CODE_IDENT_MASK                 (0x03)

/*! 控制码功能编码 */
#define CONTROL_CODE_FUNCTION_CMD               (0x00) /*!< 命令 */
#define CONTROL_CODE_FUNCTION_ACK               (0x14) /*!< 应答帧 */
#define CONTROL_CODE_FUNCTION_UPLOAD            (0x18) /*!< 主动上传帧 */

/*! 控制码识别编码 */
#define CONTROL_CODE_IDENT_CMD                  (0x00) /*!< 命令 */
#define CONTROL_CODE_IDENT_TS_HEAD              (0x01) /*!< 时序包包头 */
#define CONTROL_CODE_IDENT_TS_BODY              (0x02) /*!< 时序包包体 */
#define CONTROL_CODE_IDENT_TS_TAIL              (0x03) /*!< 时序包包尾 */

/*! 帧不需返回REP */
#define FRAME_NOT_RETURN_REP(x) (CONTROL_CODE_RETURNFLAG_MASK == ((x)&CONTROL_CODE_RETURNFLAG_MASK))
/*! 帧需返回REP */
#define FRAME_RETURN_REP(x)     (CONTROL_CODE_RETURNFLAG_MASK != ((x)&CONTROL_CODE_RETURNFLAG_MASK))

/*! 返回帧状态 */
typedef enum
{
    NONE_RETURN = 0x00, /*! 不返回 */
    NEED_ACK, /*!< 需要返回ACK */
    NEED_REP, /*!< 需要返回REP */
    NEED_EVT, /*!< 需要返回EVT */
    NEED_UPLOAD, /*!< 主动上传 */
} return_frame_state_e;

/*! 帧格式检查状态定义 */
typedef enum
{
    FRAME_CHECK_START = 0x00u,
    FRAME_CHECK_CONTROL,
    FRAME_CHECK_DATALEN,
    FRAME_CHECK_DATA,
    FRAME_CHECK_CHECKSUM,
    FRAME_CHECK_STOP,
    FRAME_CHECK_OK,
    FRAME_CHECK_ERROR
} check_state_e;

#endif // __NODEPROTOCOLRES_H
