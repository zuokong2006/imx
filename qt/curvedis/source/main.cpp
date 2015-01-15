
#include <QApplication>
#include "nodecomm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //FindDialog *dialog = new FindDialog;
    //dialog->show();
    NodeComm node;

    return app.exec();
}


