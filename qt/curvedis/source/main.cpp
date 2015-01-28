
#include <QApplication>
#include <QTimer>
#include "nodecomm.h"
#include "halcomm.h"
#include "haltemperature.h"
#include "halupgrade.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //FindDialog *dialog = new FindDialog;
    //dialog->show();
    //NodeComm node;

    //HalComm::nodeQuery(2, 0x01, 0x00, 0x00, NULL, 0, NULL, 0);
    HalComm::newNodeCommInstance();
    //HalTemperature::quireCurTemperature(1, 1);

    MainWindow *dialog = new MainWindow;
    dialog->show();

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
    //timer->start();

    return app.exec();
}


