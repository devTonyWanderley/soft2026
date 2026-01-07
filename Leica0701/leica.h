#ifndef LEICA_H
#define LEICA_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "../Banco1/geral.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Leica;}
QT_END_NAMESPACE

class Leica : public QMainWindow
{
    Q_OBJECT

public:
    Leica(QWidget *parent = nullptr);
    ~Leica();

private slots:
    void on_pbOp_clicked();

private:
    Ui::Leica *ui;
    QString Buffer;
    QSerialPort Qsp;
    uint Passo;

    void LerSerial();
    void TrataErro(QSerialPort::SerialPortError e);
};
#endif // LEICA_H
