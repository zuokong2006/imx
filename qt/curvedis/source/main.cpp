
#include <QApplication>
#include "nodecomm.h"
#include "halcomm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //FindDialog *dialog = new FindDialog;
    //dialog->show();
    //NodeComm node;
    HalComm::nodeQuery(2, 0x01, 0x00, 0x00, NULL, 0, NULL, 0);

    return app.exec();
}


