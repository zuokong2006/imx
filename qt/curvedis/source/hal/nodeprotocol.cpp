
#include <QDebug>
#include "nodeprotocol.h"
#include "nodeprotocolres.h"

class NodeProtocolPrivate
{
public:
    int framelen;
    quint8 state;
    quint8 checksum;
    quint8 datalen;
    quint8 framebuf[PROTOCOL_FRAME_MAX_SIZE];
    NodeProtocolPrivate();
    ~NodeProtocolPrivate();
    bool checkFrameStart(const quint8 data);
    bool checkFrameCtlCode(const quint8 data);
    bool checkFrameDataLen(const quint8 data);
    bool checkFrameData(const quint8 data);
    bool checkFrameCheckSum(const quint8 data);
    bool checkFrameStop(const quint8 data);
};

NodeProtocolPrivate::NodeProtocolPrivate()
{

}

NodeProtocolPrivate::~NodeProtocolPrivate()
{

}

bool NodeProtocolPrivate::checkFrameStart(const quint8 data)
{
    if(PROTOCOL_FRAME_START_CODE == data)
    {
        state = FRAME_CHECK_CONTROL;
        framelen = 0;
        framebuf[framelen] = data;
        framelen++;
        return true;
    }
    else
    {
        return false;
    }
}

bool NodeProtocolPrivate::checkFrameCtlCode(const quint8 data)
{
    switch(data)
    {
        case FRAME_CONTROL_CODE_COMMAND_REP:
        case FRAME_CONTROL_CODE_COMMAND_NO_REP:
        case FRAME_CONTROL_CODE_REP:
        case FRAME_CONTROL_CODE_ACK:
        case FRAME_CONTROL_CODE_EVT:
            state = FRAME_CHECK_DATALEN;
            checksum = data;
            framebuf[framelen] = data;
            framelen++;
            return true;
        default:
            return false;
    }
}

bool NodeProtocolPrivate::checkFrameDataLen(const quint8 data)
{
    if((3 <= data) && ((PROTOCOL_FRAME_MAX_SIZE-5) >= data))
    {
        state = FRAME_CHECK_DATA;
        datalen = data;
        checksum += data;
        framebuf[framelen] = data;
        framelen++;
        return true;
    }
    else
    {
        return false;
    }
}

bool NodeProtocolPrivate::checkFrameData(const quint8 data)
{
    if(0 < datalen)
    {
        datalen--;
        checksum += data;
        framebuf[framelen] = data;
        framelen++;
    }
    if(0 == datalen)
    {
        state = FRAME_CHECK_CHECKSUM;
    }
    return true;
}

bool NodeProtocolPrivate::checkFrameCheckSum(const quint8 data)
{
    if(data == checksum)
    {
        state = FRAME_CHECK_STOP;
        framebuf[framelen] = data;
        framelen++;
        return true;
    }
    else
    {
        return false;
    }
}

bool NodeProtocolPrivate::checkFrameStop(const quint8 data)
{
    if(PROTOCOL_FRAME_STOP_CODE == data)
    {
        state = FRAME_CHECK_OK;
        framebuf[framelen] = data;
        framelen++;
        return true;
    }
    else
    {
        return false;
    }
}


/*
 * public
 */
NodeProtocol::NodeProtocol()
{
    d = new NodeProtocolPrivate;
}

NodeProtocol::~NodeProtocol()
{
    delete d;
}

void NodeProtocol::init()
{
    d->framelen = 0;
    d->state = FRAME_CHECK_START;
    memset(d->framebuf, 0, sizeof(PROTOCOL_FRAME_MAX_SIZE));
    //qDebug() << "node protocol init()";
}

int NodeProtocol::getFrameLen()
{
    return d->framelen;
}

quint8* NodeProtocol::frameBuf() const
{
    return d->framebuf;
}

int NodeProtocol::parseProtocol(const quint8 data)
{
    bool res = 0;

    //qDebug() << "parse protocol data:" << data << ", state:" << d->state;
    switch(d->state)
    {
        case FRAME_CHECK_START:
            res = d->checkFrameStart(data);
            break;
        case FRAME_CHECK_CONTROL:
            res = d->checkFrameCtlCode(data);
            break;
        case FRAME_CHECK_DATALEN:
            res = d->checkFrameDataLen(data);
            break;
        case FRAME_CHECK_DATA:
            res = d->checkFrameData(data);
            break;
        case FRAME_CHECK_CHECKSUM:
            res = d->checkFrameCheckSum(data);
            break;
        case FRAME_CHECK_STOP:
            res = d->checkFrameStop(data);
            break;
        default:
            res = false;
            break;
    }
    //qDebug() << "next state:" << d->state;
    if(!res)
    {
        qDebug() << "check error, state:" << d->state;
        this->init();
        return -1;
    }
    if(FRAME_CHECK_OK == d->state)
    {
        d->state = FRAME_CHECK_START;
        return d->framelen;
    }
    return 0;
}



