#ifndef _SERIALTRANS_H_
#define _SERIALTRANS_H_

#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <QButtonGroup>
#include <QSocketNotifier>
#include <QScrollBar>
#include <QTimer>
#include <errno.h>
#include "serialData.h"
class SerialTrans : public QWidget
{
    Q_OBJECT
public:
    SerialTrans(QWidget * parent = NULL);
    void setSerialData(SerialData data);
    bool openSerial();
public slots:
    void closeSerial();
    void readData();
    void sendData();
    void updateData();
    void clearData();
    void startAutoSend();
signals:
    void serialClosed();
private:
    QTextEdit   * revInfo;
    QButtonGroup * checkBoxGroup;
    QCheckBox   * charBox;
    QCheckBox   * hexBox;
    QLCDNumber  * recNum;
    QPushButton * clearButton;
    QHBoxLayout * revControlLayout;

    QLineEdit   * sendInfo;
    QCheckBox   * autoSend;
    QSpinBox    * sendSpin;
    QLCDNumber  * sendNum;
    QPushButton * sendButon;
    QHBoxLayout * sendControlLayout;

    QLabel      * devLabel;
    QLabel      * baudLabel;
    QLabel      * dataLabel;
    QLabel      * parityLabel;
    QLabel      * stopLabel;
    QPushButton * closeButton;
    QHBoxLayout * comLayout;
    
    QVBoxLayout * layout;

    int fd;
    SerialData      setData;
    QString         revData;
    QTimer      *   autoTimer;
    QSocketNotifier * monitor; 
};

#endif
