
#ifndef __HALUPGRADE_H
#define __HALUPGRADE_H


namespace HalUpgrade
{
    /* 复位命令 */
    int upgradeReset(int node);
    /* 进入升级模式 */
    int enterUpgradeMode(int node);
    /* 开始升级 */
    int upgradeStart(int node, quint32 startAddr);
    /* 升级数据命令 */
    int upgradeData(int node, quint32 addr, quint8 *pbuf, int len);
    /* 升级结束 */
    int upgradeFinish(int node);
};

#endif // __HALUPGRADE_H

