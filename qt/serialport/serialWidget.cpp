#include "serialWidget.h"
#include <QDebug>
SerialWidget::SerialWidget(QWidget * parent)
    : QStackedWidget(parent)
{
    setWidget = new SerialSet;
    this->addWidget(setWidget);

    transWidget = new SerialTrans; 
    this->addWidget(transWidget);


    connect(setWidget, SIGNAL(serialDataSeted(SerialData)), this, SLOT(startTrans(SerialData)));
    connect(transWidget, SIGNAL(serialClosed()), this, SLOT(endTrans()));
}

void SerialWidget :: startTrans(SerialData data)
{
    transWidget->setSerialData(data);
    if (transWidget->openSerial() == false ) {
        return ;
    }
    this->setCurrentWidget(transWidget);
}

void SerialWidget :: endTrans()
{
    this->setCurrentWidget(setWidget);
}
