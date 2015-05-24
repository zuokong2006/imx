
#include <QDebug>
#include "halcomm.h"
#include "halsmartcard.h"

namespace HalSmartCard
{
    /* ��λPCD */
    int resetPcd(int node)
    {
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x00, NULL, 0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")reset pcd error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")reset pcd ok!";
        }
        return res;
    }

    /* �ر���Ƶ */
    int closeRf(int node)
    {
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x01, NULL, 0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")close rf error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")close rf ok!";
        }
        return res;
    }

    /* ���� */
    int test(int node, quint8 *pbuf, int len, quint8 *pret, int retLen)
    {
        if((NULL == pbuf) || (1 > len) || (NULL == pret))
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x02, pbuf, len, pret, retLen);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")smart card test error!";
        }
        else
        {
            if(res != len)
            {
                qDebug() << "(node:" << node << ")smart card test cmd return length error!";
                return -1;
            }
            qDebug() << "(node:" << node << ")smart card test ok!";
        }
        return res; //�������ݳ���
    }

    /* ���������� */
    int beep(int node, quint16 time)
    {
        quint8 param[2] = {0};
        param[0] = (quint8)(time>>8);
        param[1] = (quint8)(time);
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x03, param, 2);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")beep control error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")beep control ok!" ;
        }
        return res;
    }

    /* Ѱ�� */
    int request(int node, quint8 *ptype)
    {
        if(NULL == ptype)
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x10, NULL, 0, ptype, 1);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")smart card request error!";
        }
        else
        {
            if(1 != res)
            {
                qDebug() << "(node:" << node << ")smart card request cmd return length error!";
                return -1;
            }
            qDebug() << "(node:" << node << ")smart card request ok!";
        }
        return res;
    }

    /* ����ͻ */
    int anticoll(int node, quint8 *psnr, quint8 len)
    {
        if((NULL == psnr) || (1 > len))
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x11, NULL, 0, psnr, len);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")smart card anticoll error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")smart card anticoll ok!";
        }
        return res; //����sn����
    }

    /* ѡ�� */
    int select(int node, quint8 *psak, quint8 *psnr, quint8 len)
    {
        if((NULL == psak) || (NULL == psnr) || (1 > len))
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x12, psnr, len, psak, 1);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")smart card select error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")smart card select ok!";
        }
        return res;
    }

    /* ѡ��Ӧ������ */
    int rats(int node, quint8 param, quint8 *pres, quint8 len)
    {
        if((NULL == pres) || (1 > len))
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x13, &param, 1, pres, len);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")smart card rats error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")smart card rats ok!";
        }
        return res;
    }

    /* �������� */
    int command(int node, quint8 *psndBuf, quint8 sndLen, quint8 *precBuf, quint8 recLen)
    {
        if((NULL == psndBuf) || (1 > sndLen) || (NULL == precBuf) || (1 > recLen))
        {
            return -1;
        }
        int res = HalComm::nodeSyncSend(node, 0x02, 0x00, 0x14, \
                                        psndBuf, sndLen, precBuf, recLen);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")send smart card command error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")send smart card command ok!";
        }
        return res;
    }
};
