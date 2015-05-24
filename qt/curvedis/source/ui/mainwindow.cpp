
#include <QtGui>
#include <QDebug>
#include "mainwindow.h"
#include "temperaturewin.h"
#include "upgradewin.h"
#include "smartcardwin.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //��ʼ������
    //this->setWindowFlags(Qt::FramelessWindowHint); //ȥ��������
    this->resize(480, 272); //��ʾ��С
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

void MainWindow::on_testButton_clicked()
{
    this->hide();
    smartcardWin = new SmartcardWin;
    smartcardWin->show();
    smartcardWin->exec();
    delete smartcardWin;
    this->show();
}



