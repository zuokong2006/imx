
#include <QDebug>
#include <QtGui>
#include <QFileDialog>
#include "halupgrade.h"
#include "upgradewin.h"

#define START_ADDR      0x00008000
/*! epcs类型定义 */
typedef enum
{
    EPCS1 = 1,
    EPCS4 = 4,
    EPCS16 = 16,
    EPCS64 = 64,
    EPCS128 = 128,
    EPCS_TYPE_NUM = 5
} epcs_type_e;

class UpgradeWinPrivate
{
public:
    UpgradeWinPrivate();
    ~UpgradeWinPrivate();
    int printData(char *buf, int len);
public:
    int node; //节点号
    QString fileName;
    bool fpgaFlag;
};

UpgradeWinPrivate::UpgradeWinPrivate()
{
    fpgaFlag = false;
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
    nodeComboBox->addItem("3");
    nodeComboBox->addItem("1");
    nodeComboBox->addItem("2");
    nodeComboBox->addItem("4");
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
    if(d->fpgaFlag)
    {
        dialog->setFilter(tr("Bin Files (*.rpd)"));
    }
    else
    {
        dialog->setFilter(tr("Bin Files (*.bin)"));
    }
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
    upgradeProgressBar->setValue(0);
}

/* 发送 */
void UpgradeWin::on_sendButton_clicked()
{
    upgradeProgressBar->setValue(0);
    if(d->fpgaFlag)
    {
        this->sendRpdFile(d->fileName);
    }
    else
    {
        this->sendBinFile(d->fileName);
    }
}

/* fpga升级选择 */
void UpgradeWin::on_fpgaRadioButton_toggled(bool checked)
{
    fileEdit->clear();
    d->fpgaFlag = checked;
    if(checked)
    {
        qDebug() << "fpga radiobutton is checked!";
    }
    else
    {
        qDebug() << "fpga radiobutton is unchecked!";
    }
    upgradeProgressBar->setValue(0);
}


/* 发送MCU升级文件 */
int UpgradeWin::sendBinFile(const QString &fileName)
{
    if(fileName.isNull())
    {
        qWarning() << "file name is null!";
        return -1;
    }
    /* 打开文件 */
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "open bin file error!";
        return -1;
    }
    qint64 filesize = file.size();
    double factor = 100.0 / (double)filesize;
    /* 启动升级 */
    quint32 addr = START_ADDR;
    if(0 > HalUpgrade::upgradeStart(d->node, addr))
    {
        return -1;
    }
    /* 发送升级数据 */
    char buf[128] = {0};
    int len = 0;
    while(1)
    {
        len = file.read(buf, 128); //读取文件
        if(0 < len)
        {
            //qDebug("addr=0x%08x", addr);
            //this->printData(buf, len); //打印数据
            if(0 > HalUpgrade::upgradeData(d->node, addr, (quint8 *)buf, len))
            {
                return -1;
            }
            addr = addr + len;
        }
        if(128 > len)
        {
            break;
        }
        double progress = factor * (double)(addr-START_ADDR);
        upgradeProgressBar->setValue((int)(progress));
    }
    /* 升级结束 */
    if(0 > HalUpgrade::upgradeFinish(d->node))
    {
        return -1;
    }
    return 0;
}

/* 发送FPGA升级文件 */
int UpgradeWin::sendRpdFile(const QString &fileName)
{
    if(fileName.isNull())
    {
        qWarning() << "rpd file name is null!";
        return -1;
    }
    /* 打开文件 */
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "open rpd file error!";
        return -1;
    }
    qint64 filesize = file.size();
    double factor = 100.0 / (double)filesize;
    /* 启动FPGA升级 */
    quint32 addr = 0;
    if(0 > HalUpgrade::fpgaUpgradeStart(d->node, EPCS4, addr)) //EPCS4
    {
        return -1;
    }
    /* 发送FPGA升级数据 */
    char buf[128] = {0};
    int len = 0;
    while(1)
    {
        len = file.read(buf, 128); //读取文件
        if(0 < len)
        {
            //qDebug("addr=0x%08x", addr);
            //d->printData(buf, len); //打印数据
            if(0 > HalUpgrade::fpgaUpgradeData(d->node, addr, (quint8 *)buf, len))
            {
                return -1;
            }
            addr += len;
        }
        if(128 > len)
        {
            break;
        }
        double progress = factor * (double)addr;
        upgradeProgressBar->setValue((int)(progress));
    }
    /* FPGA升级结束 */
    if(0 > HalUpgrade::fpgaUpgradeFinish(d->node))
    {
        return -1;
    }
    return 0;
}


