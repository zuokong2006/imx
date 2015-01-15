
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
    //syncSend(); //同步发送
    //asyncSend(); //异步发送
signals:

private slots:
    void frameProcess(int node, quint8 *data, int datalen);

private:
    NodeCommPrivate *d;
};

#endif

