
#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QDialog>
#include "../../build/source/ui/ui_mainwindow.h"

class TemperatureWin;
class UpgradeWin;
class SmartcardWin;

class MainWindow : public QDialog, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tempButton_clicked();
    void on_upgradeButton_clicked();
    void on_testButton_clicked();

private:
    TemperatureWin *tempWin;
    UpgradeWin *upgradeWin;
    SmartcardWin *smartcardWin;
};

#endif // __MAINWINDOW_H

