
#include <QDebug>
#include <QtGui>
#include <QFileDialog>
#include "halupgrade.h"
#include "upgradewin.h"

#define START_ADDR      0x00008000

class UpgradeWinPrivate
{
public:
    UpgradeWinPrivate();
    ~UpgradeWinPrivate();
    int sendFile(const QString &fileName);
    int printData(char *buf, int len);
public:
    int node; //节点号
    QString fileName;
};

UpgradeWinPrivate::UpgradeWinPrivate()
{

}

UpgradeWinPrivate::~UpgradeWinPrivate()
{

}

int UpgradeWinPrivate::printData(char *buf, int len)
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

int UpgradeWinPrivate::sendFile(const QString &fileName)
{
    if(fileName.isNull())
    {
        qWarning() << "file name is null!";
        return -1;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "open bin file error!";
        return -1;
    }
    /* 启动升级 */
    quint32 addr = START_ADDR;
    if(0 > HalUpgrade::upgradeStart(node, addr))
    {
        return -1;
    }
    /* 发送试剂数据 */
    char buf[128];
    int len;
    while(1)
    {
        len = file.read(buf, 128);
        if(0 < len)
        {
            qDebug("addr=0x%08x", addr);
            this->printData(buf, len);
            if(0 > HalUpgrade::upgradeData(node, addr, (quint8 *)buf, len))
            {
                return -1;
            }
            addr = addr + len;
        }
        if(128 > len)
        {
            break;
        }
    }
    /* 升级结束 */
    if(0 > HalUpgrade::upgradeFinish(node))
    {
        return -1;
    }

    return 0;
}

/*
 * public
 */
UpgradeWin::UpgradeWin(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //初始化窗体
    //this->setWindowFlags(Qt::FramelessWindowHint); //去掉标题栏
    this->resize(480, 272); //显示大小
    /* 创建 */
    d = new UpgradeWinPrivate;
    //QFileDialog
    nodeComboBox->addItem("1");
    nodeComboBox->addItem("3");
    sendButton->setEnabled(false);
}

UpgradeWin::~UpgradeWin()
{
    delete d;
}

/* 节点选择 */
void UpgradeWin::on_nodeComboBox_currentIndexChanged(const QString & text )
{
    d->node = text.toInt();
    qDebug("node=%d", d->node);
}

/* 复位 */
void UpgradeWin::on_resetButton_clicked()
{
    HalUpgrade::upgradeReset(d->node);
}

/* 进入升级模式 */
void UpgradeWin::on_enterButton_clicked()
{
    HalUpgrade::enterUpgradeMode(d->node);
}

/* 选择文件 */
void UpgradeWin::on_browseButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle(tr("Open APP File"));
    dialog->setDirectory("/home");
    dialog->resize(400, 200);
    dialog->setFilter(tr("Bin Files (*.bin)"));
    if(dialog->exec() == QDialog::Accepted)
    {
        d->fileName = dialog->selectedFiles()[0];
        fileEdit->clear();
        fileEdit->setText(d->fileName);
        sendButton->setEnabled(true);
    }
    else
    {
        fileEdit->clear();
        sendButton->setEnabled(false);
    }
}

/* 发送 */
void UpgradeWin::on_sendButton_clicked()
{
    d->sendFile(d->fileName);
}




