/* name: can.h
 * description: CANÇý¶¯½Ó¿Ú
 * author: zuokongxiao
 * date: 2014-08-21
 */

#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>

int can_open(void);
int can_close(void);
int can_read(can_frame *pstCanFrame);
int can_write(can_frame *pstCanFrame);
int can_write_test(uint8 ucBoardType);

