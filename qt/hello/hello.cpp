 #include <QApplication>
 #include <QPushButton>
 #include <QtGui>

 int main(int argc, char *argv[])
 {
    QApplication app(argc, argv);

    QPushButton hello("Hello world!");
    hello.show();
     
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    char *info = "you should see some chinese words: 你好，世界";
    QString infoString = info;
    QLabel	label(infoString);
    label.show();
     
    return app.exec();
 }