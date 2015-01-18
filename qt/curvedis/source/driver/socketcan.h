
#ifndef __SOCKETCAN_H
#define __SOCKETCAN_H

/* 节点号定义 */
#define NODE_NUM                15

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
    int nodeSize();
    bool isNode(int node);
    int nodeIndex(int node);
private:
    SocketCanPrivate *d;
};


#endif

