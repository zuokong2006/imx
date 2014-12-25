#ifndef _SERIALWIDGET_H_
#define _SERIALWIDGET_H_

#include <QWidget>
#include <QStackedWidget>
#include "serialSet.h"
#include "serialTrans.h"
class SerialWidget : public QStackedWidget
{
    Q_OBJECT
public:
    SerialWidget(QWidget * parent = NULL);
public slots:
    void startTrans(SerialData data);
    void endTrans();
private:
    SerialSet   * setWidget;
    SerialTrans * transWidget;
};

#endif
