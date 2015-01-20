
#include <QDebug>
#include "halcomm.h"
#include "haltemperature.h"

namespace HalTemperature
{
    /* 查询当前温度 */
    float quireCurTemperature(int node, quint8 ch)
    {
        float curtemp = 0.0;
        quint8 ret[2] = {0};
        if(2 == HalComm::nodeQuery(node, 0x62, ch, 0x10, NULL, 0, ret, 2))
        {
            quint16 tempval = (ret[0] << 8) | ret[1];
            qint16 curt = (qint16)tempval;
            curtemp = curt / 100.0; //放大100倍
            qDebug() << "(node:" << node << " ch:" << ch
                     << ")quire temperature:" << curtemp;
        }
        else
        {
            qWarning() << "(node:" << node << " ch:" << ch
                       << ")quire temperature error!";
        }
        return curtemp;
    }

    /* 温度控制 */
    int temperatureCtl(int node, quint8 ch, quint8 state, float temp)
    {
        quint8 param[3] = {0};
        qint16 curt = (qint16)(temp*100);
        param[0] = state;
        param[1] = (quint8)(curt>>8);
        param[2] = (quint8)curt;
        return HalComm::nodeSyncSend(node, 0x62, ch, 0x11, param, 3);
    }

    /* 打开温度控制并设置目标值 */
    int openTemperatureCtl(int node, quint8 ch, float tarTemp)
    {
        int res = temperatureCtl(node, ch, 0x01, tarTemp);
        if(0 > res)
        {
            qWarning() << "(node:" << node << " ch:" << ch
                       << ")open temperature control error!";
        }
        else
        {
            qDebug() << "(node:" << node << " ch:" << ch
                     << ")open temperature control ok!";
        }
        return res;
    }

    /* 关闭温度控制 */
    int closeTemperatureCtl(int node, quint8 ch)
    {
        int res = temperatureCtl(node, ch, 0x00, 0.0);
        if(0 > res)
        {
            qWarning() << "(node:" << node << " ch:" << ch
                       << ")close temperature control error!";
        }
        else
        {
            qDebug() << "(node:" << node << " ch:" << ch
                     << ")close temperature control ok!";
        }
        return res;
    }

    /* 设置PID参数 */
    int setTemperaturePidParam(int node, quint8 ch, float kp, float ki, float kd)
    {
        quint8 param[12] = {0};
        qint32 lParam = 0;
        lParam = (qint32)(kp * 10000);
        param[0] = (quint8)((lParam>>24) & 0x000000FF);
        param[1] = (quint8)((lParam>>16) & 0x000000FF);
        param[2] = (quint8)((lParam>>8) & 0x000000FF);
        param[3] = (quint8)(lParam & 0x000000FF);
        lParam = (qint32)(ki * 10000);
        param[4] = (quint8)((lParam>>24) & 0x000000FF);
        param[5] = (quint8)((lParam>>16) & 0x000000FF);
        param[6] = (quint8)((lParam>>8) & 0x000000FF);
        param[7] = (quint8)(lParam & 0x000000FF);
        lParam = (qint32)(kd * 10000);
        param[8] = (quint8)((lParam>>24) & 0x000000FF);
        param[9] = (quint8)((lParam>>16) & 0x000000FF);
        param[10] = (quint8)((lParam>>8) & 0x000000FF);
        param[11] = (quint8)(lParam & 0x000000FF);
        int res = HalComm::nodeSyncSend(node, 0x62, ch, 0x20, param, 12);
        if(0 > res)
        {
            qWarning() << "(node:" << node << " ch:" << ch
                       << ")set temperature pid param error!";
        }
        else
        {
            qDebug() << "(node:" << node << " ch:" << ch
                     << ")set temperature pid param ok!" << kp << ki << kd;
        }
        return res;
    }

    /* 获取PID参数 */
    int getTemperaturePidParam(int node, quint8 ch, float *kp, float *ki, float *kd)
    {
        if((NULL == kp) || (NULL == ki) || (NULL == kd))
        {
            qWarning() << "input param error!";
            return -1;
        }
        quint8 ret[12] = {0};
        int res = HalComm::nodeQuery(node, 0x62, ch, 0x21, NULL, 0, ret, 12);
        if(12 == res)
        {
            qint32 lParam = 0;
            lParam = (((quint32)ret[0] << 24) | ((quint32)ret[1] << 16) \
                 | ((quint32)ret[2] << 8) | ((quint32)ret[3]));
            *kp = (float)lParam;
            *kp  = *kp / 10000;
            lParam = (((quint32)ret[4] << 24) | ((quint32)ret[5] << 16) \
                 | ((quint32)ret[6] << 8) | ((quint32)ret[7]));
            *ki = (float)lParam;
            *ki  = *ki / 10000;
            lParam = (((quint32)ret[8] << 24) | ((quint32)ret[9] << 16) \
                 | ((quint32)ret[10] << 8) | ((quint32)ret[11]));
            *kd = (float)lParam;
            *kd  = *kd / 10000;
            res = 0;
            qDebug() << "(node:" << node << " ch:" << ch
                     << ")get temperature pid param ok!" << *kp << *ki << *kd;
        }
        else
        {
            qWarning() << "(node:" << node << " ch:" << ch
                       << ")get temperature pid param error!";
        }
        return res;
    }
};
