
#ifndef __HALTEMPERATURE_H
#define __HALTEMPERATURE_H


namespace HalTemperature
{
    /* 查询当前温度 */
    float quireCurTemperature(int node, quint8 ch);
    /* 温度控制 */
    int temperatureCtl(int node, quint8 ch, quint8 state, float temp);
    /* 打开温度控制并设置目标值 */
    int openTemperatureCtl(int node, quint8 ch, float tarTemp);
    /* 关闭温度控制 */
    int closeTemperatureCtl(int node, quint8 ch);
    /* 设置PID参数 */
    int setTemperaturePidParam(int node, quint8 ch, float kp, float ki, float kd);
    /* 获取PID参数 */
    int getTemperaturePidParam(int node, quint8 ch, float *kp, float *ki, float *kd);
};

#endif // __HALTEMPERATURE_H

