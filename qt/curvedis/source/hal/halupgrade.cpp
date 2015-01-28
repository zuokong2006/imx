
#include <QDebug>
#include "halcomm.h"
#include "halupgrade.h"

namespace HalUpgrade
{
    /* 复位命令 */
    int upgradeReset(int node)
    {
        //异步复位
        int res = HalComm::nodeAsyncSend(node, 0x01, 0x00, 0x20, NULL, 0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")upgrade reset error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")upgrade reset ok!" ;
        }
        return res;
    }

    /* 进入升级模式 */
    int enterUpgradeMode(int node)
    {
        int res = HalComm::nodeSyncSend(node, 0x01, 0x00, 0x21, NULL, 0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")enter upgrade mode error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")enter upgrade mode!" ;
        }
        return res;
    }

    /* 开始升级 */
    int upgradeStart(int node, quint32 startAddr)
    {
        quint8 param[4] = {0};
        param[0] = (quint8)(startAddr>>24);
        param[1] = (quint8)(startAddr>>16);
        param[2] = (quint8)(startAddr>>8);
        param[3] = (quint8)(startAddr);
        int res = HalComm::nodeSyncSend(node, 0x01, 0x00, 0x22, param, 4);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")start upgrade error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")upgrade start!" ;
        }
        return res;
    }

    /* 升级数据命令 */
    int upgradeData(int node, quint32 addr, quint8 *pbuf, int len)
    {
        if((NULL == pbuf) || (128 != len))
        {
            return -1;
        }
        quint8 param[132] = {0};
        param[0] = (quint8)(addr>>24);
        param[1] = (quint8)(addr>>16);
        param[2] = (quint8)(addr>>8);
        param[3] = (quint8)(addr);
        memcpy(param+4, pbuf, len);
        int res = HalComm::nodeSyncSend(node, 0x01, 0x00, 0x23, param, 132);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")send upgrade data error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")send upgrade data ok!" ;
        }
        return res;
    }

    /* 升级结束 */
    int upgradeFinish(int node)
    {
        int res = HalComm::nodeSyncSend(node, 0x01, 0x00, 0x24, NULL, 0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << ")send upgrade finish cmd error!";
        }
        else
        {
            qDebug() << "(node:" << node << ")upgrade finish!" ;
        }
        return res;
    }
};
