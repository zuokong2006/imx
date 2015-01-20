
#ifndef __HALCOMM_H
#define __HALCOMM_H


class NodeComm;

namespace HalComm
{
    extern NodeComm *nodeComm;
    void newNodeCommInstance();
    /* 查询 */
    int nodeQuery(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen);
    /* 同步发送 */
    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, int timeout=500);
    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                 int timeout=500);
    int nodeSyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout=500);
    /* 异步发送 */
    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, int timeout=500);
    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                  int timeout=500);
    int nodeAsyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout=500);
};

#endif // __HALCOMM_H

