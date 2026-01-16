#ifndef CFGCOM_H
#define CFGCOM_H

#include <QDialog>
#include <QTimer>
#include <QStyle>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QDateTime>

namespace Ui {class cfgCom;}

class cfgCom : public QDialog
{
    Q_OBJECT

public:
    QSerialPort cSerial;
    bool flg;

    explicit cfgCom(QWidget *parent = nullptr);
    ~cfgCom();

private slots:
    void on_pbOk_clicked();

    void on_pbCancela_clicked();

private:
    Ui::cfgCom *ui;

    void iniCombos();
    bool atuPorts();
    bool cfgPorta();
};

#endif // CFGCOM_H
