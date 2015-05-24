
#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>

#include <QDebug>
#include <QList>
#include "socketcan.h"

#ifndef AF_CAN
#define AF_CAN 29
#endif
#ifndef PF_CAN
#define PF_CAN AF_CAN
#endif

/* CAN���ı�ʶ������ */
/* Դ�ڵ��ַmask */
#define SRC_ID_MASK             0x07C00000
/* Ŀ��ڵ��ַmask */
#define DEST_ID_MASK            0x001F0000
/* �ֶ���mask */
#define SEG_CODE_MASK           0x0000FF00
/* �ֶα�־mask */
#define SEG_CODE_FLAG_MASK      0x0000C000
/* �ֶκ�mask */
#define SEG_CODE_NUM_MASK       0x00003F00
/* �ֶα�־ */
#define FRAME_NONE_SEG          0x00000000 // �޷ֶ�
#define FRAME_FIRST_SEG         0x00004000 // ��һ���ֶ�
#define FRAME_MIDDLE_SEG        0x00008000 // �м�ֶ�
#define FRAME_END_SEG           0x0000C000 // ���ֶ�

#define errout(_s)  printf("error class: %s\n", (_s))
#define errcode(_d) printf("error code: %02x\n", (_d))

class SocketCanPrivate
{
public:
    int canfd;
    QList<int> nodeList;
    SocketCanPrivate();
    ~SocketCanPrivate();
    int writeFrame(struct can_frame *pframe);
    int printFrame(struct can_frame *pframe);
    int printErrFrame(struct can_frame *pframe);
};

SocketCanPrivate::SocketCanPrivate()
{
    nodeList.append(1);
    nodeList.append(2);
    nodeList.append(3);
    nodeList.append(4);
    nodeList.append(5);
}

SocketCanPrivate::~SocketCanPrivate()
{

}

int SocketCanPrivate::writeFrame(struct can_frame *pframe)
{
    if((canfd <= 0) || (NULL == pframe))
    {
        printf("E:input param error!\r\n");
        return -1;
    }
    if(0 > write(canfd, pframe, sizeof(*pframe)))
    {
        printf("E: can write failed!\r\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

int SocketCanPrivate::printFrame(struct can_frame *pframe)
{
    int i;
    char tmp[32] = { 0 }, buf[32] = { 0 };

    for(i = 0; i < pframe->can_dlc; i++)
    {
        sprintf(tmp, "%02x ", pframe->data[i]);
        strcat(buf, tmp);
    }
    printf("ID=0x%08x, DLC=%d, DATA=%s\r\n", \
        pframe->can_id, pframe->can_dlc, buf);
}

int SocketCanPrivate::printErrFrame(struct can_frame *pframe)
{
    if(pframe->can_id & CAN_ERR_TX_TIMEOUT) {
        errout("CAN_ERR_TX_TIMEOUT");
    }
    if(pframe->can_id & CAN_ERR_LOSTARB) {
        errout("CAN_ERR_LOSTARB");
        errcode(pframe->data[0]);
    }
    if(pframe->can_id & CAN_ERR_CRTL) {
        errout("CAN_ERR_CRTL");
        errcode(pframe->data[1]);
    }
    if(pframe->can_id & CAN_ERR_PROT) {
        errout("CAN_ERR_PROT");
        errcode(pframe->data[2]);
        errcode(pframe->data[3]);
    }
    if(pframe->can_id & CAN_ERR_TRX) {
        errout("CAN_ERR_TRX");
        errcode(pframe->data[4]);
    }
    if(pframe->can_id & CAN_ERR_ACK) {
        errout("CAN_ERR_ACK");
    }
    if(pframe->can_id & CAN_ERR_BUSOFF) {
        errout("CAN_ERR_BUSOFF");
    }
    if(pframe->can_id & CAN_ERR_BUSERROR) {
        errout("CAN_ERR_BUSERROR");
    }
    if(pframe->can_id & CAN_ERR_RESTARTED) {
        errout("CAN_ERR_RESTARTED");
    }
}

/*******************************************************************************
* public
*******************************************************************************/
SocketCan::SocketCan()
{
    d = new SocketCanPrivate;
}

SocketCan::~SocketCan()
{
    delete d;
}

int SocketCan::openCan()
{
    int ret;
    struct sockaddr_can addr;
    struct ifreq ifr;

    d->canfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(d->canfd < 0)
    {
        qWarning() << "socket PF_CAN failed!";
        return -1;
    }
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(d->canfd, SIOCGIFINDEX, &ifr);
    if(ret < 0)
    {
        qWarning() << "ioctl failed!";
        return -1;
    }
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(d->canfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0)
    {
        qWarning() << "bind failed!";
        return -1;
    }
#if 1
    /* CAN����������
     * kernel\Documentation\networking\can.txt
     * <received_can_id> & mask == can_id & mask */
    struct can_filter filter[1];
    filter[0].can_id = (NODE_NUM<<16) | CAN_EFF_FLAG;
    filter[0].can_mask = (NODE_NUM<<16);
    qDebug("id:0x%08x mask:0x%08x", filter[0].can_id, filter[0].can_mask);
    ret = setsockopt(d->canfd, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
#else
    ret = setsockopt(d->canfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
#endif
    if(ret < 0)
    {
        qWarning() << "setsockopt failed!";
        return -1;
    }
    qDebug() << "SocketCan: open can!";

    return 0;
}

int SocketCan::closeCan()
{
    close(d->canfd);
    qWarning() << "SocketCan: close can!";
    return 0;
}

int SocketCan::getFd()
{
    return d->canfd;
}

/* ��CAN���߶�ȡ���� */
int SocketCan::readData(int &node, char *pdata, int maxlen)
{
    int ret = 0;
    struct can_frame canframebuf = {0};

    if((d->canfd <= 0) || (NULL == pdata))
    {
        qWarning() << "input param error!";
        return -1;
    }
    if(0 > (ret = read(d->canfd, &canframebuf, sizeof(struct can_frame))))
    {
        qWarning() << "read can error!";
        return ret;
    }
    /* �ж��豸�Ƿ���� */
    if(canframebuf.can_id & CAN_ERR_FLAG)
    {
        d->printErrFrame(&canframebuf);
        qWarning() << "CAN device error!";
        return -1;
    }
    /* ��ȡ����Դ��ַ */
    node = (canframebuf.can_id >> 22) & 0x1f;
    if(maxlen < canframebuf.can_dlc)
    {
        qWarning() << "datalen overflow!";
        return -1;
    }
    memcpy(pdata, canframebuf.data, canframebuf.can_dlc);
    /* ��ӡCAN����֡ */
    d->printFrame(&canframebuf);

    return canframebuf.can_dlc;
}

int SocketCan::writeData(int node, const char *pdata, int datalen)
{
    struct can_frame stCanFrame;
    int i = 0;
    unsigned char ucCnt = 0;
    unsigned short unTemp = 0;

    if((NULL == pdata) || (1 > datalen))
    {
        qWarning() << "input param error!";
        return -1;
    }
    /*! ����������Ҫ��������֡ */
    unTemp = datalen / 8;
    if(0x0000 != (datalen & 0x0007))
    {
        unTemp = unTemp + 1;
    }
    /*! ���ݲ�����8�ֽڣ�����ֶ� */
    if(1 == unTemp)
    {
        stCanFrame.can_id = 0x80000000 \
                            | (NODE_NUM << 22) | (node << 16) |FRAME_NONE_SEG;
        stCanFrame.can_dlc = datalen;
        for(i = 0; i < datalen; i++)
        {
            stCanFrame.data[i] = pdata[i];
        }
        if(0 > d->writeFrame(&stCanFrame))
        {
            return -1;
        }
    }
    else
    {
        /*! ��һ���ֶα��� */
        stCanFrame.can_id = 0x80000000 \
                            | (NODE_NUM << 22) | (node << 16) | FRAME_FIRST_SEG;
        stCanFrame.can_dlc = 8;
        for(i = 0; i<8; i++)
        {
            stCanFrame.data[i] = *pdata;
            pdata++;
        }
        if(0 > d->writeFrame(&stCanFrame))
        {
            return -1;
        }
        unTemp--;
        ucCnt = 1;
        /*! �м�ֶα��� */
        while(1 != unTemp)
        {
            stCanFrame.can_id = 0x80000000 \
                                | (NODE_NUM << 22) | (node << 16) \
                                | FRAME_MIDDLE_SEG | (ucCnt << 8);
            stCanFrame.can_dlc = 8;
            for(i = 0; i<8; i++)
            {
                stCanFrame.data[i] = *pdata;
                pdata++;
            }
            if(0 > d->writeFrame(&stCanFrame))
            {
                return -1;
            }
            unTemp--;
            ucCnt++;
        }
        /*! ���ֶα��� */
        if(0x0000 == (datalen & 0x0007))
        {
            unTemp = 8;
        }
        else
        {
            unTemp = datalen & 0x0007;
        }
        ucCnt = 0;
        stCanFrame.can_id = 0x80000000 \
                            | (NODE_NUM << 22) | (node << 16) \
                            | FRAME_END_SEG | (ucCnt << 8);
        stCanFrame.can_dlc = unTemp;
        for(i = 0; i<unTemp; i++)
        {
            stCanFrame.data[i] = *pdata;
            pdata++;
        }
        if(0 > d->writeFrame(&stCanFrame))
        {
            return -1;
        }
    }

    return datalen;
}

int SocketCan::nodeSize()
{
    return d->nodeList.size();
}

bool SocketCan::isNode(int node)
{
    QList<int>::const_iterator i = d->nodeList.begin();
    while(i != d->nodeList.end())
    {
        if(*i == node)
        {
            return true;
        }
        i++;
    }

    return false;
}

int SocketCan::nodeIndex(int node)
{
    int n = 0;
    QList<int>::const_iterator i = d->nodeList.begin();
    while(i != d->nodeList.end())
    {
        if(*i == node)
        {
            return n;
        }
        i++;
        n++;
    }

    return -1;
}

