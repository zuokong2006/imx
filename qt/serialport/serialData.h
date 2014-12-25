#ifndef _SERIALDATA_H_
#define _SERIALDATA_H_
#include <termios.h>

struct SerialData 
{
    QString devName; 
    QString baudRate;
    QString dataBit;
    QString parity;
    QString stopBit;
};

#endif
