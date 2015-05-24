
#ifndef __SMARTCARDWIN_H
#define __SMARTCARDWIN_H

#include <QDialog>
#include "../../../build/source/ui/smartcard/ui_smartcardwin.h"

class SmartcardWinPrivate;

class SmartcardWin : public QDialog, public Ui::SmartcardWin
{
    Q_OBJECT

public:
    SmartcardWin(QWidget *parent = 0);
    ~SmartcardWin();

private slots:
    //void on_nodeComboBox_currentIndexChanged(const QString & text );
    void on_testButton_clicked();
    void on_beepButton_clicked();

private:
    SmartcardWinPrivate *d;
};


#endif // __SMARTCARDWIN_H
