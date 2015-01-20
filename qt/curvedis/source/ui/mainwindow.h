
#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QDialog>
#include "../../build/source/ui/ui_mainwindow.h"

class MainWindow : public QDialog, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    //~MainWindow();

};

#endif // __MAINWINDOW_H

