#ifndef SERIAL_H
#define SERIAL_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class Serial;
}

class Serial : public QDialog
{
    Q_OBJECT

public:

    QString *Buf;

    explicit Serial(QWidget *parent = nullptr);
    ~Serial();

private slots:
    void on_pbOpera_clicked();
    void Lido();

private:
    Ui::Serial *ui;

    QSerialPort *Qsp;
};

#endif // SERIAL_H
