
#ifndef __NODEPROTOCOL_H
#define __NODEPROTOCOL_H


class NodeProtocolPrivate;

class NodeProtocol
{
public:
    NodeProtocol();
    ~NodeProtocol();
    void init();
    int getFrameLen();
    quint8 *frameBuf() const;
    int parseProtocol(const quint8 data);

private:
    NodeProtocolPrivate *d;
};


#endif // __NODEPROTOCOL_H
