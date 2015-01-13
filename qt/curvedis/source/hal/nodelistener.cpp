
#include <sys/types.h>
#include <sys/time.h>
#include <QDebug>
#include "nodelistener.h"
#include "socketcan.h"


NodeListener::NodeListener()
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
    
    for(;;)
    {
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        fd_set rdfds;
        FD_ZERO(&rdfds);
        int fd = this->can->getFd();
        FD_SET(fd, &rdfds);
        int res = select(fd+1, &rdfds, NULL, NULL, &timeout);
        if(0 < res)
        {
            if(FD_ISSET(fd, &rdfds))
            {
                int rdlen = this->can->readData(node, buf, 16);
                if(0 < rdlen)
                {
                    /* 解析数据 */
                    
                    
                    //emit frameReceived(node, );
                }
                else
                {
                    qDbug() << "node listener read data error!";
                }
            }
        }
        else if(0 > res)
        {
            qDbug() << "node listener select return error!";
        }
        else
        {
            //qDbug() << "select timeout!";
        }
    }
}

