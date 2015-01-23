
#ifndef __TEMPERATUREWIN_H
#define __TEMPERATUREWIN_H

#include <QDialog>
#include "../../../build/source/ui/temperature/ui_temperaturewin.h"

class TemperatureWinPrivate;

class TemperatureWin : public QDialog, public Ui::TemperatureWin
{
    Q_OBJECT

public:
    TemperatureWin(QWidget *parent = 0);
    ~TemperatureWin();

private slots:
    void on_returnButton_clicked();
    void on_startstopButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_nodeComboBox_currentIndexChanged(const QString & text );
    void on_chComboBox_currentIndexChanged(const QString & text );
    void on_settingButton_clicked();

protected:
    void timerEvent(QTimerEvent *event);

private:
    TemperatureWinPrivate *d;
};


#endif // __TEMPERATUREWIN_H

