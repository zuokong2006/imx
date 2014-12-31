
#ifndef _GOTOCELLDIALOG_H
#define _GOTOCELLDIALOG_H

#include <QDialog>
#include "ui_gotocelldialog.h"

/* ¿‡∂®“Â */
class GoToCellDialog : public QDialog, public Ui::GoToCellDialog
{
    Q_OBJECT
    
public:
    GoToCellDialog(QWidget *parent = 0);
    
private slots:
    void on_lineEdit_textChanged();
};


#endif

