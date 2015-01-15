
#include <QDebug>
#include "nodecomm.h"
#include "socketcan.h"
#include "nodelistener.h"


class NodeCommPrivate
{
public:
    NodeCommPrivate();
    ~NodeCommPrivate();
    SocketCan *can;
    NodeListener *listener;
    int asyncSend(quint8 node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, int paramLen); //异步发送
    int syncSend(quint8 node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, int paramLen, \
                 quint8 *retData, int retLen); //同步发送

};

NodeCommPrivate::NodeCommPrivate()
{
    can = new SocketCan;
    can->openCan();
    listener = new NodeListener;
    listener->setCan(can);
    listener->start();
    char buf[8] = {0xaa, 0x00, 0x03, 0x01, 0xf0, 0x01, 0xf5, 0x55};
    can->writeData(2, buf, 8);
    can->writeData(5, buf, 8);
    can->writeData(2, buf, 8);
}

NodeCommPrivate::~NodeCommPrivate()
{
    can->closeCan();
    delete listener;
    delete can;
}

int NodeCommPrivate::asyncSend(quint8 node, quint8 ctlCode, quint8 cmdType, \
            quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, int paramLen)
{

}

int NodeCommPrivate::syncSend(quint8 node, quint8 ctlCode, quint8 cmdType, \
                        quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, \
                        int paramLen, quint8 *retData, int retLen)
{

}

/*******************************************************************************
* public
*******************************************************************************/
NodeComm::NodeComm()
{
    d = new NodeCommPrivate;
}

NodeComm::~NodeComm()
{
    delete d;
}




