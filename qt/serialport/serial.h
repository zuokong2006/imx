#ifndef __SERIAL_H_
#define __SERIAL_H_

#include <qobject.h>

class QSocketNotifier;

class SERIAL : public QObject
{
    Q_OBJECT
public:
    SERIAL(QWidget *parent = 0, const char *name = 0,
            const char *filename = 0);
    ~SERIAL();
    int set_option(int bps, int nbit, int stop);

public slots:
    void readserial(int type);
    void setstopbit(int index);
    void setbps(int index);

signals:
    void getMessage(QString *str);

private:
    int fd;
    char buf[1024];
    QSocketNotifier *readnotif;
};

#endif
