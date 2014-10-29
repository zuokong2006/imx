/* name: printerlib.h
 * description: 打印机库
 * author: zuokongxiao
 * date: 2014-10-23
 */

#ifndef __PRINTERLIB_H
#define __PRINTERLIB_H


/* 打印机状态定义 */
typedef enum 
{
    PRINTER_NORMAL = 0x00, /* 打印机正常  */
    PRINTER_ONLINE = 0x00, /* 联机 */
    PRINTER_OFFLINE = 0x08, /* 脱机 */
    PRINTER_NOT_OUT_OF_PAPER = 0x00, /* 打印机不缺纸 */
    PRINTER_OUT_OF_PAPER = 0x20, /* 打印机缺纸 */
    PRINTER_HEAD_NORMAL = 0x00, /* 打印机机头正常 */
    PRINTER_HEAD_OVERHEAT = 0x40, /* 打印机机头过热 */
} printer_state_e;


void printer_fildes(int fd);
int printer_init(void);
int set_printer_linespacing(unsigned char ucLineNum);
int printer_paper_feed(unsigned char ucLineNum);
int get_printer_state(unsigned char *pucState);
int printer_picture_print(unsigned char *pucBuf, int lSize);
int printer_bmp_print(char *pcBmpName);

#endif

