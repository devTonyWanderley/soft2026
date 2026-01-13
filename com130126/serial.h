#ifndef SERIAL_H
#define SERIAL_H

//  configurar receptor
//  disparar receptor
//  disparar transmissor pré configurado
//  salvar arquivo

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QStyle>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class Serial;}
QT_END_NAMESPACE

class Serial : public QMainWindow
{
    Q_OBJECT

public:
    Serial(QWidget *parent = nullptr);
    ~Serial();

private:
    Ui::Serial *ui;
};
#endif // SERIAL_H
