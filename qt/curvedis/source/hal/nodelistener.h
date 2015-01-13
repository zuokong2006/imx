
#ifndef __NODELISTENER_H
#define __NODELISTENER_H

#include <QThread>

class SocketCan;

class NodeListener : public QThread
{
    Q_OBJECT
public:
    NodeListener();
    ~NodeListener();
    void setCan(SocketCan *can);
    
signals:
    void frameReceived(int node, char *data, int datalen);
    
protected:
    void run();

private:
    SocketCan *can;
};

#endif

