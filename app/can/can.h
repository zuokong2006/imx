/* name: can.h
 * description: CAN�����ӿ�
 * author: zuokongxiao
 * date: 2014-08-21
 */
#ifndef CAN_H
#define CAN_H



int can_open(void);
int can_close(void);
int can_read(struct can_frame *pstCanFrame);
int can_write(struct can_frame *pstCanFrame);
int can_write_test(uint8 ucBoardType);

#endif