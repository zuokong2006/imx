#include "serialTrans.h"
SerialTrans :: SerialTrans(QWidget * parent)
    : QWidget(parent)
{
    revInfo = new QTextEdit; 
    revInfo->setMinimumSize(5,5);
    revInfo->setReadOnly(true);

    charBox = new QCheckBox("char");
    hexBox  = new QCheckBox("hex");
    checkBoxGroup = new QButtonGroup;
    checkBoxGroup->addButton(charBox);
    checkBoxGroup->addButton(hexBox);
    recNum  = new QLCDNumber;
    clearButton = new QPushButton("clear");

    revControlLayout = new QHBoxLayout;
    revControlLayout->addWidget(charBox);
    revControlLayout->addWidget(hexBox);
    revControlLayout->addWidget(recNum);
    revControlLayout->addWidget(clearButton);

    sendInfo = new QLineEdit;
    autoSend = new QCheckBox("auto");
    sendSpin = new QSpinBox;
    sendSpin->setSuffix("ms");
    sendSpin->setRange(1, 10000);
    sendNum  = new QLCDNumber;
    sendButon = new QPushButton("send");

    sendControlLayout = new QHBoxLayout;
    sendControlLayout->addWidget(autoSend);
    sendControlLayout->addWidget(sendSpin);
    sendControlLayout->addWidget(sendNum);
    sendControlLayout->addWidget(sendButon);

    devLabel    = new QLabel;
    baudLabel   = new QLabel;
    dataLabel   = new QLabel;
    parityLabel = new QLabel;
    stopLabel   = new QLabel;
    closeButton = new QPushButton("close");

    devLabel->setAlignment(Qt::AlignCenter);
    baudLabel->setAlignment(Qt::AlignCenter);
    dataLabel->setAlignment(Qt::AlignCenter);
    parityLabel->setAlignment(Qt::AlignCenter);
    stopLabel->setAlignment(Qt::AlignCenter);
    
    comLayout   = new QHBoxLayout();
    comLayout->addWidget(devLabel);
    comLayout->addWidget(baudLabel);
    comLayout->addWidget(dataLabel);
    comLayout->addWidget(parityLabel);
    comLayout->addWidget(stopLabel);
    comLayout->addWidget(closeButton);


    layout = new QVBoxLayout;
    layout->addWidget(revInfo);
    layout->addLayout(revControlLayout);
    layout->addWidget(sendInfo);
    layout->addLayout(sendControlLayout);
    layout->addLayout(comLayout);
    this->setLayout(layout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeSerial()));
    connect(checkBoxGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateData()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearData()));
    connect(sendButon, SIGNAL(clicked()), this, SLOT(sendData()));
    connect(autoSend, SIGNAL(clicked()), this, SLOT(startAutoSend()));
}

void SerialTrans :: setSerialData(SerialData data)
{
    setData = data;
    devLabel->setText(setData.devName);
    baudLabel->setText(setData.baudRate);
    dataLabel->setText(setData.dataBit);
    parityLabel->setText(setData.parity);
    stopLabel->setText(setData.stopBit);
}

bool SerialTrans :: openSerial()
{
    struct termios setting;
    fd = open(qPrintable("/dev/" + setData.devName), O_RDWR);
    if (fd == -1) {
        QMessageBox::warning(this, "open error", strerror(errno));
        goto openerr;
    }
    tcgetattr(fd, &setting); 
    //  设置波特率
    speed_t speed;
    if (setData.baudRate == "B4800") {
        speed = B4800; 
    } else if (setData.baudRate == "B9600") {
        speed = B9600; 
    } else if (setData.baudRate == "B19200") {
        speed = B19200; 
    } else if (setData.baudRate == "B38400") {
        speed = B38400; 
    } else if (setData.baudRate == "B57600") {
        speed = B57600; 
    } else if (setData.baudRate == "B115200") {
        speed = B115200; 
    } else if (setData.baudRate == "B230400") {
        speed = B230400;
    } else if (setData.baudRate == "B460800") {
        speed = B460800;
    } else {
        QMessageBox::warning(this, "error", "speed error!"); 
        goto seterr;
    }
    cfsetispeed(&setting, speed);
    cfsetospeed(&setting, speed);
    cfmakeraw(&setting);
    //  设置数据位 
    if (setData.dataBit == "8") {
        setting .c_cflag |= CS8;
    } else if (setData.dataBit == "7") {
        setting.c_cflag |= CS7; 
    } else if (setData.dataBit == "6") {
        setting.c_cflag |= CS6;
    } else if (setData.dataBit == "5") {
        setting.c_cflag |= CS5; 
    } else {
        QMessageBox::warning(this, "error", "dataBit error!");  
        goto seterr;
    }
    //  设置parity
    if (setData.parity == "none") {
        setting.c_cflag &= ~PARENB;
        setting.c_iflag &= ~INPCK;
    } else if (setData.parity == "odd") {
        setting.c_cflag |= (PARODD | PARENB); 
        setting.c_iflag |= INPCK;
    } else if (setData.parity == "even") {
        setting.c_cflag |= PARENB; 
        setting.c_cflag &= ~PARODD;
        setting.c_iflag |= INPCK;
    } else {
        QMessageBox::warning(this, "error", "dataBit error!");  
        goto seterr;
    }
    //  设置停止位
    if (setData.stopBit == "1") {
        setting.c_cflag &= ~CSTOPB; 
    } else if (setData.stopBit == "2") {
        setting.c_cflag |= CSTOPB; 
    } else {
        QMessageBox::warning(this, "error", "stopBit error!");  
        goto seterr;
    }
    
    tcflush(fd, TCIFLUSH);
    setting.c_cc[VTIME] = 0;
    setting.c_cc[VMIN] = 0;
    tcsetattr(fd, TCSANOW, &setting);

    monitor = new QSocketNotifier(fd, QSocketNotifier::Read, NULL);
    autoTimer = new QTimer;

    connect(monitor, SIGNAL(activated(int)), this, SLOT(readData()));
    connect(autoTimer, SIGNAL(timeout()), this, SLOT(sendData()));

    monitor->setEnabled(true);
    charBox->setCheckState(Qt::Checked);
    autoSend->setCheckState(Qt::Unchecked);
    sendSpin->setValue(100);
    sendInfo->clear();
    clearData();

    return true;
seterr:
    ::close(fd);
openerr:
    return false;
}


void SerialTrans :: startAutoSend()
{
    if (autoSend->isChecked()) {
        sendSpin->setDisabled(true);
        autoTimer->start(sendSpin->value());
    } else {
        autoTimer->stop(); 
        sendSpin->setDisabled(false);
    }
}

void SerialTrans :: sendData()
{
    int ret;
    ret = write(fd, sendInfo->text().toAscii(), sendInfo->text().size());
    sendNum->display(sendNum->intValue() + ret);
}

void SerialTrans :: clearData()
{
    revData.clear();
    recNum->display(0);
    sendNum->display(0);
    updateData();
}

void SerialTrans :: updateData()
{
    if (hexBox->isChecked()) {
        int i;
        QString hexStr;
        for (i = 0; i < revData.size(); i++) {
            hexStr += QString::number(revData.at(i).toAscii(), 16) + " "; 
        }
        revInfo->setText(hexStr);
    } else {
        revInfo->setText(revData); 
    }
}

void SerialTrans :: readData()
{
    char buf[2000];
    int  ret;
    int  i;
    while (revData.size() > 99999) {
        revData.remove(0, 1000); 
    }
    ret = read(fd, buf, sizeof(buf));
    revData  += QByteArray::fromRawData(buf, ret);

    if (hexBox->isChecked()) {
        QString hexStr;
        for (i = 0; i < revData.size(); i++) {
            hexStr += QString::number(revData.at(i).toAscii(), 16) + " "; 
        }
        revInfo->setText(hexStr);
    } else {
        revInfo->setText(revData);   
    }
    recNum->display(revData.size()); 
    revInfo->verticalScrollBar()->setValue(revInfo->verticalScrollBar()->maximumHeight());
}

void SerialTrans :: closeSerial()
{
    ::close(fd);
    autoTimer->stop();
    delete monitor;
    delete autoTimer;
    emit serialClosed();
}
