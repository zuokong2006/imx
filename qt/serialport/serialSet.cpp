#include "serialSet.h"

#define SPEEDSTR_LEN 15

SerialSet :: SerialSet(QWidget * parent)
    : QWidget(parent)
{


    comBox     = new QComboBox();
    comLabel   = new QLabel("com");
    QString  str;
    int i;


    str = "ttyAM0" ;
    comBox->addItem(str);

    for (i = 0; i < 5; i++) {
        str = "ttySP" + QString::number(i);
        comBox->addItem(str);
    }
    comBox->setCurrentIndex(comBox->findText("ttyAM0"));

    baudBox    = new QComboBox();
    baudLabel  = new QLabel("baudrate");
    QStringList baudStr;
    baudStr << "B4800" << "B9600" << "B19200" \
            << "B38400" << "B57600" << "B115200" \
            << "B230400" << "B460800";
    for (i = 0; i < baudStr.size(); i++) {
        baudBox->addItem(baudStr.at(i));
    }
    baudBox->setCurrentIndex(baudBox->findText("B115200"));

    dataBitBox    = new QComboBox();
    dataBitLabel  = new QLabel("databit");
    for (i = 5; i < 9; i++) {
        dataBitBox->addItem(QString::number(i)); 
    }
    dataBitBox->setCurrentIndex(dataBitBox->findText("8"));

    parityBox   = new QComboBox();
    parityLabel = new QLabel("parity");
    QStringList parityStr;
    parityStr << "none" << "odd" << "even";
    for (i = 0; i < parityStr.size(); i++) {
        parityBox->addItem(parityStr.at(i)); 
    }
    parityBox->setCurrentIndex(parityBox->findText("none"));

    stopBitBox   = new QComboBox();
    stopBitLabel = new QLabel("stopbit");
    QStringList stopBitStr;
    stopBitStr << "1" << "2";
    for (i = 0; i < stopBitStr.size(); i++) {
        stopBitBox->addItem(stopBitStr.at(i)); 
    }
    stopBitBox->setCurrentIndex(stopBitBox->findText("1"));

    openButton = new QPushButton("open");
    connect(openButton, SIGNAL(clicked()), this, SLOT(setSerialData()));

    leftLayout = new QGridLayout;
    leftLayout->addWidget(comBox, 0, 1);
    leftLayout->addWidget(comLabel, 0, 0);
    leftLayout->addWidget(baudBox, 1, 1);
    leftLayout->addWidget(baudLabel, 1, 0);
    leftLayout->addWidget(dataBitBox, 2, 1);
    leftLayout->addWidget(dataBitLabel, 2, 0);
    leftLayout->addWidget(parityBox, 3, 1);
    leftLayout->addWidget(parityLabel, 3, 0);
    leftLayout->addWidget(stopBitBox, 4, 1);
    leftLayout->addWidget(stopBitLabel, 4, 0);
    leftLayout->addWidget(openButton,5,1);
    this->setLayout(leftLayout);
}

void SerialSet :: setSerialData()
{
    SerialData data;
    data.devName  = comBox->currentText();
    data.baudRate = baudBox->currentText();
    data.dataBit  = dataBitBox->currentText();
    data.parity   = parityBox->currentText();
    data.stopBit  = stopBitBox->currentText();
    emit serialDataSeted(data);
}
