
#if 0 //hello test
#include <QApplication>
#include <QPushButton>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton hello("Hello world!");
    hello.show();
    QObject::connect(&hello, SIGNAL(clicked()), &app, SLOT(quit()));
    
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    char *info = "you should see some chinese words: 你好，世界";
    QString infoString = info;
    QLabel label(infoString);
    label.show();
    
    return app.exec();
}
#else

#include <QApplication>
#include <QSpinBox>
#include <QSlider>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Enter Your Age");

    QSpinBox *spinBox = new QSpinBox;
    QSlider *slider = new QSlider(Qt::Horizontal);
    spinBox->setRange(0, 130);
    slider->setRange(0, 130);

    QObject::connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    spinBox->setValue(35);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox);
    layout->addWidget(slider);
    window->setLayout(layout);

    window->show();

    return app.exec();
}

#endif

