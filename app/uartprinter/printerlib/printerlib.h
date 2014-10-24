/* name: printerlib.h
 * description: ´òÓ¡»ú¿â
 * author: zuokongxiao
 * date: 2014-10-23
 */

#ifndef __PRINTERLIB_H
#define __PRINTERLIB_H


void printer_fildes(int fd);
int set_printer_linespacing(unsigned char ucLineNum);
int printer_picture_print(unsigned char *pucBuf, int lSize);
int printer_bmp_print(char *pcBmpName);

#endif

