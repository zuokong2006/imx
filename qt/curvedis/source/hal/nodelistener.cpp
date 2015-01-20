
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <QDebug>
#include "socketcan.h"
#include "nodelistener.h"
#include "nodeprotocol.h"
#include "nodeprotocolres.h"


#define BUF_SIZE            512u


NodeListener::NodeListener() : QThread()
{

}

NodeListener::~NodeListener()
{

}

void NodeListener::setCan(SocketCan *can)
{
    this->can = can;
}

void NodeListener::run()
{
    int node = 0;
    char buf[16];
    int nodesize = this->can->nodeSize();
    int wrindex[nodesize], rdindex[nodesize];
    quint8 framebuf[nodesize][BUF_SIZE];
    NodeProtocol parse[nodesize];
    for(node=0; node<nodesize; node++)
    {
        parse[node].init();
        wrindex[node] = 0;
        rdindex[node] = 0;
    }
    qDebug() << "Listener run, nodesize=" << nodesize;

    for(;;)
    {
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        fd_set rdfds;
        FD_ZERO(&rdfds);
        int fd = this->can->getFd();
        FD_SET(fd, &rdfds);
        //qDebug() << "select";
        int res = select(fd+1, &rdfds, NULL, NULL, &timeout);
        if(0 < res)
        {
            //qDebug() << "select res > 0";
            if(FD_ISSET(fd, &rdfds))
            {
                int rdlen = this->can->readData(node, buf, 16);
                //qDebug() << "readData() len=" << rdlen;
                if((0 < rdlen) && (this->can->isNode(node)))
                {
                    int index = this->can->nodeIndex(node);
                    //qDebug() << "index=" << index;
                    if(0 <= index)
                    {
                        int i = 0, wrrdindex = 0;
                        for(i=0; i<rdlen; i++)
                        {
                            wrrdindex = wrindex[index];
                            //qDebug() << "wrindex=" << wrrdindex;
                            framebuf[index][wrrdindex] = buf[i];
                            (wrindex[index])++;
                            if(BUF_SIZE <= wrindex[index])
                                wrindex[index] = 0;
                        }
                        while(wrindex[index] != rdindex[index])
                        {
                            wrrdindex = rdindex[index];
                            //qDebug() << "rdindex=" << wrrdindex;
                            res = parse[index].parseProtocol(framebuf[index][wrrdindex]);
                            (rdindex[index])++;
                            if(BUF_SIZE <= rdindex[index])
                                rdindex[index] = 0;
                            if(0 < res)
                            {
                                emit frameReceived(node, parse[index].frameBuf(), res);
                                //qDebug() << node << res;
                            }
                        }
                    }
                }
                else
                {
                    qDebug() << "node listener read data error!";
                }
            }
        }
        else if(0 > res)
        {
            qDebug() << "node listener select return error!";
        }
        else
        {
            //qDebug() << "select timeout!";
        }
    }
}

