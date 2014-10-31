/* name: thermalprinterlib.h
 * description: 热敏打印机库
 * author: zuokongxiao
 * date: 2014-10-30
 */

#ifndef __THERMALPRINTERLIB_H
#define __THERMALPRINTERLIB_H


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


/* 热敏打印机类 */
class thermalprinter
{
    public:
        int open_dev(const char *pcTtyName);
        int close_dev(void);
        int init(void); /* 初始化命令 */
        int set_linespacing(unsigned char ucLineNum); /* 设置行间距 */
        int paper_feed(unsigned char ucLineNum); /* 走纸 */
        int get_state(unsigned char *pucState); /* 获取打印机状态 */
        int print_picture(unsigned char *pucBuf, int lSize); /* 打印图形 */
        int print_1bit_bmp(const char *pcBmpName);
    private:
        int lPrinterFd; /* 打印机文件描述符 */
};


#endif

