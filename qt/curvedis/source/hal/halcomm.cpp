
#include <QDebug>
#include "nodecomm.h"
#include "halcomm.h"

namespace HalComm
{
    NodeComm *nodeCommInstance()
    {
        static NodeComm* instance = new NodeComm();
        return instance;
    }

    /* 查询 */
    int nodeQuery(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen)
    {
        return nodeCommInstance()->query(node, cmdType, cmdNum, cmdContent, \
                        cmdParam, paramLen, retData, retLen);
    }

    /* 同步发送 */
    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, int timeout)
    {
        return nodeCommInstance()->syncSend(node, cmdType, cmdNum, cmdContent, \
                        cmdParam, paramLen, timeout);
    }

    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                 int timeout)
    {
        return nodeCommInstance()->syncSend(node, cmdType, cmdNum, cmdContent, \
                        cmdParam, paramLen, retData, retLen, timeout);
    }

    int nodeSyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
    {
        return nodeCommInstance()->syncSend(node, ctlCode, cmdType, cmdNum, \
                        cmdContent, cmdParam, paramLen, timeout);
    }

    /* 异步发送 */
    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, int timeout)
    {
        return nodeCommInstance()->asyncSend(node, cmdType, cmdNum, cmdContent, \
                        cmdParam, paramLen, timeout);
    }

    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                  int timeout)
    {
        return nodeCommInstance()->asyncSend(node, cmdType, cmdNum, cmdContent, \
                        cmdParam, paramLen, retData, retLen, timeout);
    }

    int nodeAsyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
    {
        return nodeCommInstance()->asyncSend(node, ctlCode, cmdType, cmdNum, \
                        cmdContent, cmdParam, paramLen, timeout);
    }
};
