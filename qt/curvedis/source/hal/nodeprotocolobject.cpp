
#include <QDebug>
#include <QSemaphore>
#include "nodeprotocolres.h"
#include "nodeprotocolobject.h"


class NodeProtocolObjectPrivate
{
public:
    NodeProtocolObjectPrivate();
    ~NodeProtocolObjectPrivate();
    int nodeNum;
    quint8 ctlCode;
    quint8 cmdType;
    quint8 cmdNum;
    quint8 cmdContent;
    int cmdPramLen;
    quint8 cmdParam[PROTOCOL_FRAME_MAX_SIZE];
    QSemaphore sem;
    void init();
    bool isMatch(const NodeProtocolObjectPrivate &object);
};

NodeProtocolObjectPrivate::NodeProtocolObjectPrivate()
{
    init();
}

NodeProtocolObjectPrivate::~NodeProtocolObjectPrivate()
{

}

void NodeProtocolObjectPrivate::init()
{
    nodeNum = 0;
    ctlCode = 0;
    cmdType = 0;
    cmdNum = 0;
    cmdContent = 0;
    cmdPramLen = 0;
    memset(cmdParam, 0, sizeof(cmdParam));
    if(sem.available() > 0)
    {
        sem.acquire(1);
    }
}

bool NodeProtocolObjectPrivate::isMatch(const NodeProtocolObjectPrivate& object)
{
    if((nodeNum == object.nodeNum) && (cmdType == object.cmdType) \
       && (cmdNum == object.cmdNum) && (cmdContent == object.cmdContent))
    {
        return true;
    }
    return false;
}

/*
 * public
 */
NodeProtocolObject::NodeProtocolObject()
{
    d = new NodeProtocolObjectPrivate;
}

NodeProtocolObject::NodeProtocolObject(const NodeProtocolObject &object)
{
    d = new NodeProtocolObjectPrivate;
    d->nodeNum = object.d->nodeNum;
    d->ctlCode = object.d->ctlCode;
    d->cmdType = object.d->cmdType;
    d->cmdNum = object.d->cmdNum;
    d->cmdContent = object.d->cmdContent;
    d->cmdPramLen = object.d->cmdPramLen;
    memcpy(d->cmdParam, object.d->cmdParam, sizeof(d->cmdParam));
}

NodeProtocolObject::~NodeProtocolObject()
{
    delete d;
}

int NodeProtocolObject::nodeNum() const
{
    return d->nodeNum;
}

quint8 NodeProtocolObject::ctlCode() const
{
    return d->ctlCode;
}

quint8 NodeProtocolObject::cmdType() const
{
    return d->cmdType;
}

quint8 NodeProtocolObject::cmdNum() const
{
    return d->cmdNum;
}

quint8 NodeProtocolObject::cmdContent() const
{
    return d->cmdType;
}

void NodeProtocolObject::setNodeNum(int nodeNum)
{
    d->nodeNum = nodeNum;
}

void NodeProtocolObject::setCtlCode(quint8 ctlCode)
{
    d->ctlCode = ctlCode;
}

void NodeProtocolObject::setCmdType(quint8 cmdType)
{
    d->cmdType = cmdType;
}

void NodeProtocolObject::setCmdNum(quint8 cmdNum)
{
    d->cmdNum = cmdNum;
}

void NodeProtocolObject::setCmdContent(quint8 cmdContent)
{
    d->cmdContent = cmdContent;
}

int NodeProtocolObject::cmdParamLen() const
{
    return d->cmdPramLen;
}

quint8 *NodeProtocolObject::cmdParams() const
{
    return d->cmdParam;
}

quint8 NodeProtocolObject::cmdParam(int i)
{
    return (i < d->cmdPramLen) ? d->cmdParam[i] : 0;
}

void NodeProtocolObject::setCmdParam(const quint8 *cmdParam, int paramLen)
{
    if(0 == paramLen)
    {
        d->cmdPramLen = 0;
        memset(d->cmdParam, 0, sizeof(d->cmdParam));
    }
    else
    {
        d->cmdPramLen = (paramLen < PROTOCOL_FRAME_MAX_SIZE) \
                        ? paramLen : PROTOCOL_FRAME_MAX_SIZE;
        memcpy(d->cmdParam, cmdParam, d->cmdPramLen);
    }
}

bool NodeProtocolObject::wait(int timeout)
{
    return d->sem.tryAcquire(1, timeout);
}

void NodeProtocolObject::release()
{
    d->sem.release();
}

void NodeProtocolObject::init()
{
    d->init();
}

bool NodeProtocolObject::isMatch(const NodeProtocolObject &object)
{
    return d->isMatch(*(object.d));
}

NodeProtocolObject& NodeProtocolObject::operator=(const NodeProtocolObject& other)
{
    d->nodeNum = other.d->nodeNum;
    d->ctlCode = other.d->ctlCode;
    d->cmdType = other.d->cmdType;
    d->cmdNum = other.d->cmdNum;
    d->cmdContent = other.d->cmdContent;
    d->cmdPramLen = other.d->cmdPramLen;
    memcpy(d->cmdParam, other.d->cmdParam, sizeof(d->cmdParam));

    return *this;
}

