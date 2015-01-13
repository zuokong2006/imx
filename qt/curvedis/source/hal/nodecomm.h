
#ifndef __NODECOMM_H
#define __NODECOMM_H

#include <QObject>

class NodeCommPrivate;

class NodeComm : public QObject
{
    Q_OBJECT
public:
    NodeComm();
    ~NodeComm();
    //int 
    syncSend(); //同步发送
    asyncSend(); //异步发送
signals:
    
    
private slots:

private:
    NodeCommPrivate *d;
};

#endif

