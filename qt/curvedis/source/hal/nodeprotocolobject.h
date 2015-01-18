
#ifndef __NODEPROTOCOLOBJECT_H
#define __NODEPROTOCOLOBJECT_H

class NodeProtocolObjectPrivate;

class NodeProtocolObject
{
public:
    NodeProtocolObject();
    NodeProtocolObject(const NodeProtocolObject &object);
    ~NodeProtocolObject();
    int nodeNum() const;
    quint8 ctlCode() const;
    quint8 cmdType() const;
    quint8 cmdNum() const;
    quint8 cmdContent() const;
    void setNodeNum(int nodeNum);
    void setCtlCode(quint8 ctlCode);
    void setCmdType(quint8 cmdType);
    void setCmdNum(quint8 cmdNum);
    void setCmdContent(quint8 cmdContent);
    int cmdParamLen() const;
    quint8 *cmdParams() const;
    quint8 cmdParam(int i);
    void setCmdParam(const quint8 *cmdParam, int paramLen);
    bool wait(int timeout);
    void release();
    void init();
    bool isMatch(const NodeProtocolObject &object);
    NodeProtocolObject& operator=(const NodeProtocolObject &other);
private:
    NodeProtocolObjectPrivate *d;
};

#endif // __NODEPROTOCOLOBJECT_H
