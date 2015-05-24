
#include <QDebug>
#include <QtGui>
#include <QFileDialog>
#include "halsmartcard.h"
#include "smartcardwin.h"


#define NODE        4

class SmartcardWinPrivate
{
public:
    SmartcardWinPrivate();
    ~SmartcardWinPrivate();
    int printData(char *buf, int len);
public:
    //const int node = 4; //节点号
};

SmartcardWinPrivate::SmartcardWinPrivate()
{

}

SmartcardWinPrivate::~SmartcardWinPrivate()
{

}

int SmartcardWinPrivate::printData(char *buf, int len)
{
    if(NULL == buf)
    {
        qWarning() << "buf ponter is null!";
        return -1;
    }
    if(0 == len)
    {
        return 0;
    }
    QString str;
    for(int i=0; i<len; i++)
    {
        str += QString("%1 ").arg(buf[i], 2, 16, QChar('0'));
        if(0 == ((i+1)%16))
        {
            str += "\r\n";
        }
    }
    qDebug() << str;
    return len;
}

/*
 * public
 */
SmartcardWin::SmartcardWin(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //初始化窗体
    //this->setWindowFlags(Qt::FramelessWindowHint); //去掉标题栏
    this->resize(480, 272); //显示大小
    /* 创建 */
    d = new SmartcardWinPrivate;
    //QFileDialog
    //nodeComboBox->addItem("3");
    //nodeComboBox->addItem("1");
    //nodeComboBox->addItem("2");
    //nodeComboBox->addItem("4");
    //sendButton->setEnabled(false);
}

SmartcardWin::~SmartcardWin()
{
    delete d;
}

/* 节点选择 */
//void UpgradeWin::on_nodeComboBox_currentIndexChanged(const QString & text )
//{
//    d->node = text.toInt();
//    qDebug("node=%d", d->node);
//}

/* test */
void SmartcardWin::on_testButton_clicked()
{
    quint8 buf[5] = {0x01, 0x20, 0x03, 0x40, 0x05};
    quint8 ret[5] = {0};

    int res = HalSmartCard::test(NODE, buf, 5, ret, 5);
    if(0 < res)
    {
        qDebug() << "Test command ret data: ";
        d->printData((char *)buf, 5);
    }
}

/* beep */
void SmartcardWin::on_beepButton_clicked()
{
    HalSmartCard::beep(NODE, 200); //2s
}




