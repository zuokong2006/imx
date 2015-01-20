
#include <QtGui>
#include <QDebug>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //初始化窗体

    //this->setWindowFlags(Qt::FramelessWindowHint); //去掉标题栏
    this->resize(480, 272); //显示大小
}

//MainWindow::~MainWindow()
//{

//}


