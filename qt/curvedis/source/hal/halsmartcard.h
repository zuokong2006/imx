
#ifndef __HALSMARTCARD_H
#define __HALSMARTCARD_H


namespace HalSmartCard
{
    /* ¸´Î»PCD */
    int resetPcd(int node);
    /* ¹Ø±ÕÉäÆµ */
    int closeRf(int node);
    /* ²âÊÔ */
    int test(int node, quint8 *pbuf, int len, quint8 *pret, int retLen);
    /* ·äÃùÆ÷¿ØÖÆ */
    int beep(int node, quint16 time);

    /* Ñ°¿¨ */
    int request(int node, quint8 *ptype);
    /* ·À³åÍ» */
    int anticoll(int node, quint8 *psnr, quint8 len);
    /* Ñ¡¿¨ */
    int select(int node, quint8 *psak, quint8 *psnr, quint8 len);
    /* Ñ¡ÖĞÓ¦´ğÇëÇó */
    int rats(int node, quint8 param, quint8 *pres, quint8 len);
    /* ´«ÊäÃüÁî */
    int command(int node, quint8 *psndBuf, quint8 sndLen, quint8 *precBuf, quint8 recLen);
};

#endif // __HALSMARTCARD_H

