
#ifndef __NODECOMM_H
#define __NODECOMM_H

#include <QObject>

class NodeComm : public QOBject
{
    Q_OBJECT
public:
    NodeComm();
    ~NodeComm();
    
    syncSend(); //同步发送
    asyncSend(); //异步发送
signals:
    
    
private slots:
    
};

#endif

