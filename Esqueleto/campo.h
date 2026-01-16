#ifndef CAMPO_H
#define CAMPO_H

#define SDIR "C:\\Tony\\Soft\\soft2026\\Inst\\"

#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>
#include "../Banco3/cfgcom.h"

//  Variável Serial

QT_BEGIN_NAMESPACE
namespace Ui {class Campo;}
QT_END_NAMESPACE

class Campo : public QMainWindow
{
    Q_OBJECT

public:
    Campo(QWidget *parent = nullptr);
    ~Campo();

private slots:
    void on_cbEdita_checkStateChanged(const Qt::CheckState &arg1);
    void on_pbCfg_clicked();

private:
    Ui::Campo *ui;
    cfgCom *cfgS;
    QSerialPort *pontSerial;
    bool *pontFlg;

    void opera();
};
#endif // CAMPO_H
