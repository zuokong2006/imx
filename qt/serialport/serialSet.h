#ifndef _SERIALSET_H_
#define _SERIALSET_H_


#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "serialData.h"
class SerialSet : public QWidget
{
    Q_OBJECT
public:
    SerialSet(QWidget * parent = NULL);
public slots:
    void setSerialData();
signals:
    void serialDataSeted(SerialData data);
private:
    QComboBox * comBox; 
    QComboBox * baudBox; 
    QComboBox * dataBitBox; 
    QComboBox * parityBox; 
    QComboBox * stopBitBox;

    QLabel    * comLabel;
    QLabel    * baudLabel;
    QLabel    * dataBitLabel;
    QLabel    * parityLabel;
    QLabel    * stopBitLabel;

    QPushButton * openButton;

    QGridLayout * leftLayout;
};

#endif
