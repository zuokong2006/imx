
#include <QDebug>
#include <QtGui>
#include "haltemperature.h"
#include "tempsettingwin.h"


TempSettingWin::TempSettingWin(QWidget *parent, int node, quint8 ch)
    : QDialog(parent)
{
    setupUi(this);
    this->node = node;
    this->ch = ch;

    targetSpinBox->setValue(1.0);
    kpSpinBox->setValue(0.0);
    kiSpinBox->setValue(0.0);
    kdSpinBox->setValue(0.0);
}

TempSettingWin::~TempSettingWin()
{

}

/* 获取PID参数 */
void TempSettingWin::on_getPidParamButton_clicked()
{
    float kp, ki, kd;
    if(0 == HalTemperature::getTemperaturePidParam(node, ch, &kp, &ki, &kd))
    {
        kpSpinBox->setValue(kp);
        kiSpinBox->setValue(ki);
        kdSpinBox->setValue(kd);
    }
}

/* 设置PID参数 */
void TempSettingWin::on_setPidParamButton_clicked()
{
    float kp, ki, kd;
    kp = kpSpinBox->value();
    ki = kiSpinBox->value();
    kd = kdSpinBox->value();
    HalTemperature::setTemperaturePidParam(node, ch, kp, ki, kd);
}

/* 打开温度控制 */
void TempSettingWin::on_openButton_clicked()
{
    float target = targetSpinBox->value();
    qDebug("target temperature:%f.", target);
    HalTemperature::openTemperatureCtl(node, ch, target);
}

/* 关闭温度控制 */
void TempSettingWin::on_closeButton_clicked()
{
    HalTemperature::closeTemperatureCtl(node, ch);
}

