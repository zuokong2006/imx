
#ifndef __HALSMARTCARD_H
#define __HALSMARTCARD_H


namespace HalSmartCard
{
    /* ��λPCD */
    int resetPcd(int node);
    /* �ر���Ƶ */
    int closeRf(int node);
    /* ���� */
    int test(int node, quint8 *pbuf, int len, quint8 *pret, int retLen);
    /* ���������� */
    int beep(int node, quint16 time);

    /* Ѱ�� */
    int request(int node, quint8 *ptype);
    /* ����ͻ */
    int anticoll(int node, quint8 *psnr, quint8 len);
    /* ѡ�� */
    int select(int node, quint8 *psak, quint8 *psnr, quint8 len);
    /* ѡ��Ӧ������ */
    int rats(int node, quint8 param, quint8 *pres, quint8 len);
    /* �������� */
    int command(int node, quint8 *psndBuf, quint8 sndLen, quint8 *precBuf, quint8 recLen);
};

#endif // __HALSMARTCARD_H

