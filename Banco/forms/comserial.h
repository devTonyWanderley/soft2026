#ifndef COMSERIAL_H
#define COMSERIAL_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class ComSerial;
}

class ComSerial : public QDialog
{
    Q_OBJECT

public:
    QByteArray *Buf;

    explicit ComSerial(QWidget *parent = nullptr);
    ~ComSerial();

private slots:
    void on_pbAbre_clicked();

private:
    Ui::ComSerial *ui;
    QSerialPort *Qsp;

    void Ler();
    void Erro(QSerialPort::SerialPortError e);
};

#endif // COMSERIAL_H
