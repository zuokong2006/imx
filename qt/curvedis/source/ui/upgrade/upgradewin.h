
#ifndef __UPGRADEWIN_H
#define __UPGRADEWIN_H

#include <QDialog>
#include "../../../build/source/ui/upgrade/ui_upgradewin.h"

class UpgradeWinPrivate;

class UpgradeWin : public QDialog, public Ui::UpgradeWin
{
    Q_OBJECT

public:
    UpgradeWin(QWidget *parent = 0);
    ~UpgradeWin();

private slots:
    void on_nodeComboBox_currentIndexChanged(const QString & text );
    void on_resetButton_clicked();
    void on_enterButton_clicked();
    void on_browseButton_clicked();
    void on_sendButton_clicked();

private:
    UpgradeWinPrivate *d;
};


#endif // __UPGRADEWIN_H
