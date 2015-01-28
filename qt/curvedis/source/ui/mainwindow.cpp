
#include <QtGui>
#include <QDebug>
#include "mainwindow.h"
#include "temperaturewin.h"
#include "upgradewin.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //初始化窗体
    //this->setWindowFlags(Qt::FramelessWindowHint); //去掉标题栏
    this->resize(480, 272); //显示大小
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_tempButton_clicked()
{
    this->hide();
    tempWin = new TemperatureWin;
    tempWin->show();
    tempWin->exec();
    delete tempWin;
    this->show();
}

void MainWindow::on_upgradeButton_clicked()
{
    this->hide();
    upgradeWin = new UpgradeWin;
    upgradeWin->show();
    upgradeWin->exec();
    delete upgradeWin;
    this->show();
}


