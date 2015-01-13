
#ifndef __SOCKETCAN_H
#define __SOCKETCAN_H

class SocketCanPrivate;

class SocketCan
{
public:
    SocketCan();
    ~SocketCan();
    int openCan();
    int closeCan();
    int getFd();
    int readData(int &node, char *pdata, int maxlen);
    int writeData(int node, const char *pdata, int datalen);
    int getNodeNum();
    bool isNode(int node);
private:
    SocketCanPrivate *d;
};


#endif

