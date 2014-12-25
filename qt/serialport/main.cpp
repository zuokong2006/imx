#include <QApplication>
#include "serialWidget.h"
int main(int argc,char * argv[])
{
    QApplication app(argc, argv);
    SerialWidget    widget;
    widget.showMaximized();
//    widget.showFullScreen();
    return app.exec();
}

