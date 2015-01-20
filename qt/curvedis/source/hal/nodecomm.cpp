
#include <QDebug>
#include <QMutex>
#include <QByteArray>
#include <QSemaphore>
#include "nodecomm.h"
#include "socketcan.h"
#include "nodelistener.h"
#include "nodeprotocolres.h"
#include "nodeprotocolobject.h"


class NodeCommPrivate
{
public:
    NodeCommPrivate();
    ~NodeCommPrivate();
    SocketCan *can;
    NodeListener *listener;
    QMutex mutex;
    int curIndex;
    QByteArray recByteArray;
    QList<NodeProtocolObject*> asynObjects;
    NodeProtocolObject objects[128];
    NodeProtocolObject *allocProtocolObject();
    quint8 ClacChecksum(quint8 ctlCode, quint8 cmdLen, quint8 cmdType, \
            quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, int paramLen);
    int cmdGenerate(quint8 *cmdBuf, quint8 cmdLen, int node, \
                    quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                    quint8 cmdContent, quint8 *cmdParam, int paramLen);
    int toObject(int node, const quint8 *data, int datalen, \
                 NodeProtocolObject *object);
    int asyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, int paramLen);
    int syncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, \
                 quint8 retCtlCode, quint8 *retData, int retLen, int timeout);
};

NodeCommPrivate::NodeCommPrivate()
{
    can = new SocketCan;
    can->openCan();
    listener = new NodeListener;
    listener->setCan(can);
    curIndex = 0;
}

NodeCommPrivate::~NodeCommPrivate()
{
    can->closeCan();
    delete listener;
    delete can;
}

NodeProtocolObject *NodeCommPrivate::allocProtocolObject()
{
    NodeProtocolObject* object = 0;
    mutex.lock();
    object = &objects[curIndex];
    curIndex++;
    if (128 <= curIndex)
    {
        curIndex = 0;
    }
    mutex.unlock();
    object->init();

    return object;
}

quint8 NodeCommPrivate::ClacChecksum(quint8 ctlCode, quint8 cmdLen, quint8 cmdType, \
            quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, int paramLen)
{
    quint8 checksum = 0;
    checksum = ctlCode + cmdLen + cmdType + cmdNum + cmdContent;
    for(int i=0; i<paramLen; i++)
    {
        checksum += *cmdParam;
        cmdParam++;
    }

    return checksum;
}

int NodeCommPrivate::cmdGenerate(quint8 *cmdBuf, quint8 cmdLen, int node, \
                            quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                            quint8 cmdContent, quint8 *cmdParam, int paramLen)
{
    if(cmdLen < (8+paramLen))
    {
        qWarning() << "cmd buffer len overflow.";
        return -1;
    }
    cmdBuf[FRAME_START_INDEX] = PROTOCOL_FRAME_START_CODE;
    cmdBuf[FRAME_CONTROL_INDEX] = ctlCode;
    cmdBuf[FRAME_DATALEN_INDEX] = 3 + paramLen;
    cmdBuf[FRAME_COMMAND_TYPE_INDEX] = cmdType;
    cmdBuf[FRAME_COMMAND_NUM_INDEX] = cmdNum;
    cmdBuf[FRAME_COMMAND_CONTENT_INDEX] = cmdContent;
    for(int i=0; i<paramLen; i++)
    {
        cmdBuf[FRAME_COMMAND_PARAM_INDEX+i] = cmdParam[i];
    }
    cmdBuf[FRAME_COMMAND_PARAM_INDEX+paramLen] \
                = ClacChecksum(ctlCode, 3+paramLen, cmdType, \
                               cmdNum, cmdContent, cmdParam, paramLen);
    cmdBuf[FRAME_COMMAND_PARAM_INDEX+paramLen+1] = PROTOCOL_FRAME_STOP_CODE;

    return (8+paramLen);
}

int NodeCommPrivate::toObject(int node, const quint8 *data, int datalen, \
                              NodeProtocolObject *object)
{
    if((NULL == data) || (8 > datalen) || (NULL == object))
    {
        qWarning() << "decode input param error!";
        return -1;
    }
    int frameDataLen = data[FRAME_DATALEN_INDEX];
    if(frameDataLen != (datalen-5))
    {
        qWarning() << "frame data len error!";
        return -1;
    }
    quint8 ctlCode = data[FRAME_CONTROL_INDEX];
    quint8 cmdType = data[FRAME_COMMAND_TYPE_INDEX];
    quint8 cmdNum = data[FRAME_COMMAND_NUM_INDEX];
    quint8 cmdContent = data[FRAME_COMMAND_CONTENT_INDEX];
    const quint8 *cmdParam = &(data[FRAME_COMMAND_PARAM_INDEX]);
    int cmdParamLen = frameDataLen - 3;

    object->setNodeNum(node);
    object->setCtlCode(ctlCode);
    object->setCmdType(cmdType);
    object->setCmdNum(cmdNum);
    object->setCmdContent(cmdContent);
    object->setCmdParam(cmdParam, cmdParamLen);

    return 0;
}

int NodeCommPrivate::asyncSend(int node, quint8 ctlCode, quint8 cmdType, \
            quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, int paramLen)
{
    int res = -1;
    quint8 *frameBuf = NULL;

    do
    {
        int frameLen = paramLen+8;
        frameBuf = (quint8 *)malloc(frameLen);
        if(NULL == frameBuf)
        {
            qWarning() << "malloc frame buf error!";
            break;
        }
        memset(frameBuf, 0, frameLen);
        if(frameLen != this->cmdGenerate(frameBuf, frameLen, node, ctlCode, \
                        cmdType, cmdNum, cmdContent, cmdParam, paramLen))
        {
            break;
        }
        if(frameLen != can->writeData(node, (const char *)frameBuf, frameLen))
        {
            qWarning() << "malloc frame buf error!";
            break;
        }
        res = 0;
    }while(0);
    if(NULL != frameBuf)
    {
        free(frameBuf);
    }
    qDebug() << "asyncSend complete!";

    return res;
}

int NodeCommPrivate::syncSend(int node, quint8 ctlCode, quint8 cmdType, \
        quint8 cmdNum, quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, \
        quint8 retCtlCode, quint8 *retData, int retLen, int timeout)
{
    int res = -1;

    NodeProtocolObject* asynObject = this->allocProtocolObject();
    asynObject->setNodeNum(node);
    asynObject->setCtlCode(retCtlCode);
    asynObject->setCmdType(cmdType);
    asynObject->setCmdNum(cmdNum);
    asynObject->setCmdContent(cmdContent);
    this->mutex.lock();
    this->asynObjects.append(asynObject);
    this->mutex.unlock();
    res = asyncSend(node, ctlCode, cmdType, cmdNum, cmdContent, cmdParam, paramLen);
    if(0 == res)
    {
        if(asynObject->wait(timeout))
        {
            if(FRAME_CONTROL_CODE_EVT != asynObject->ctlCode())
            {
                res = qMin(retLen, asynObject->cmdParamLen());
                if((NULL != retData) && (0 != res))
                {
                    memcpy(retData, asynObject->cmdParams(), res);
                }
            }
            else
            {
                if(2 == asynObject->cmdParamLen())
                {
                    quint16 errcode = (asynObject->cmdParam(0)) << 8;
                    errcode &= 0xff00;
                    errcode |= (asynObject->cmdParam(1));
                    qDebug("return evt errcode=0x%x", errcode);
                    res = -errcode;
                }
                else
                {
                    qWarning() << "return evt code formate error!";
                    res = -1;
                }
            }
        }
        else
        {
            qWarning() << "syncSend wait failure!";
            res = -1;
        }
    }
    else
    {
        qWarning() << "node send data failed!";
    }
    this->asynObjects.removeOne(asynObject);

    return res;
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

/* 启动节点通信 */
void NodeComm::start()
{
    d->listener->start();
    connect(d->listener, SIGNAL(frameReceived(int, quint8*, int)), \
            this, SLOT(frameReady(int, quint8*, int)), Qt::DirectConnection);
    //d->asyncSend(2, 0x00, 0x01, 0x00, 0x00, NULL, 0);
    //d->asyncSend(5, 0x00, 0x01, 0x00, 0x00, NULL, 0);
    //d->asyncSend(2, 0x00, 0x01, 0x00, 0x00, NULL, 0);

    quint8 buf[16];
    int res = this->query(1, 0x01, 0x00, 0x01, NULL, 0, buf, 16);
    if(0 < res)
    {
        QString str;
        for(int i=0; i<res; i++)
        {
            str += QString("0x%1 ").arg(buf[i], 2, 16, QChar('0'));
        }
        qDebug() << "ret data: " << str;
    }
}

/* 查询节点 */
int NodeComm::query(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen)
{
    return d->syncSend(node, FRAME_CONTROL_CODE_COMMAND_REP, \
                       cmdType, cmdNum, cmdContent, cmdParam, paramLen, \
                       FRAME_CONTROL_CODE_REP, retData, retLen, 500);
}

/* 同步发送 */
int NodeComm::syncSend(int node, quint8 cmdType, quint8 cmdNum, \
        quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
{
    return this->syncSend(node, cmdType, cmdNum, cmdContent, \
                          cmdParam, paramLen, NULL, 0, timeout);
}

int NodeComm::syncSend(int node, quint8 cmdType, quint8 cmdNum, \
                       quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, \
                       quint8 *retData, int retLen, int timeout)
{
    return d->syncSend(node, FRAME_CONTROL_CODE_COMMAND_REP, \
                       cmdType, cmdNum, cmdContent, cmdParam, paramLen, \
                       FRAME_CONTROL_CODE_REP, retData, retLen, timeout);
}

int NodeComm::syncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
{
    return d->syncSend(node, ctlCode, cmdType, cmdNum, cmdContent, \
                cmdParam, paramLen, FRAME_CONTROL_CODE_REP, NULL, 0, timeout);
}

/* 异步发送 */
int NodeComm::asyncSend(int node, quint8 cmdType, quint8 cmdNum, \
        quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
{
    return this->asyncSend(node, cmdType, cmdNum, cmdContent, \
                           cmdParam, paramLen, NULL, 0, timeout);
}

int NodeComm::asyncSend(int node, quint8 cmdType, quint8 cmdNum, \
                        quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, \
                        quint8 *retData, int retLen, int timeout)
{
    return d->syncSend(node, FRAME_CONTROL_CODE_COMMAND_REP, \
                       cmdType, cmdNum, cmdContent, cmdParam, paramLen, \
                       FRAME_CONTROL_CODE_ACK, retData, retLen, timeout);
}

int NodeComm::asyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout)
{
    return d->syncSend(node, ctlCode, cmdType, cmdNum, cmdContent, \
                cmdParam, paramLen, FRAME_CONTROL_CODE_ACK, NULL, 0, timeout);
}

/* 接收到数据帧 */
void NodeComm::frameReady(int node, quint8 *data, int datalen)
{
    QString str;
    for(int i=0; i<datalen; i++)
    {
        str += QString("0x%1 ").arg(data[i], 2, 16, QChar('0'));
    }
    qDebug() << "frameReady node:" << node << ", data:" << str;

    /* 帧解码 */
    NodeProtocolObject* object = d->allocProtocolObject();
    if(0 > d->toObject(node, data, datalen, object))
    {
        qWarning() << "convert to nodeprotocolobject error!";
        return;
    }
    /* 故障上报 */
    if(object->ctlCode() == FRAME_CONTROL_CODE_EVT)
    {
        //emit EVT
    }
    /* 同步发送返回判断 */
    d->mutex.lock();
    foreach(NodeProtocolObject *syncObject, d->asynObjects)
    {
        if(((object->ctlCode() == FRAME_CONTROL_CODE_EVT) \
                || (object->ctlCode() == syncObject->ctlCode())) \
           && (syncObject->isMatch(*object)))
        {
            syncObject->setCtlCode(object->ctlCode());
            syncObject->setCmdParam(object->cmdParams(), object->cmdParamLen());
            syncObject->release();
            break;
        }
    }
    d->mutex.unlock();
}


