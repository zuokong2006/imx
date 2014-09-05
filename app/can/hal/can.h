/* name: can.h
 * description: CAN驱动接口
 * author: zuokongxiao
 * date: 2014-08-21
 */
#ifndef __CAN_H
#define __CAN_H

/* CAN报文标识符定义 */
/* 源节点地址mask */
#define SRC_ID_MASK             0x07C00000
/* 目标节点地址mask */
#define DEST_ID_MASK            0x001F0000
/* 分段码mask */
#define SEG_CODE_MASK           0x0000FF00
/* 分段标志mask */
#define SEG_CODE_FLAG_MASK      0x0000C000
/* 分段号mask */
#define SEG_CODE_NUM_MASK       0x00003F00
/* 分段标志 */
#define FRAME_NONE_SEG          0x00000000 // 无分段
#define FRAME_FIRST_SEG         0x00004000 // 第一个分段
#define FRAME_MIDDLE_SEG        0x00008000 // 中间分段
#define FRAME_END_SEG           0x0000C000 // 最后分段


int can_open(void);
int can_close(void);
int can_read(struct can_frame *pstCanFrame);
int can_write(struct can_frame *pstCanFrame);
int can_write_test(uint8 ucBoardType);
int send_data_to_can(int lBoardType, char *pcCmd, int lCmdLen);

#endif
