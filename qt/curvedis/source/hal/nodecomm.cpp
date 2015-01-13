
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
};

NodeCommPrivate::NodeCommPrivate()
{
    can = new SocketCan;
    can->openCan();
    listener = new NodeListener;
}

NodeCommPrivate::~NodeCommPrivate()
{
    can->closeCan();
    delete listener;
    delete can;
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




