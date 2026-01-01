#include "serial.h"
#include "ui_serial.h"

Serial::Serial(QWidget *parent): QDialog(parent), ui(new Ui::Serial)
{
    ui->setupUi(this);
    QStringList qsl = {"9600", "19200", "38400", "57600", "115200"};
    ui->cbBaud->addItems(qsl);
    const QList<QSerialPortInfo> qspi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &qsi : qspi)
        ui->cbPort->addItem(qsi.portName());
}

void Serial::Lido()
{
    QByteArray dados = Qsp->readAll();
    *Buf = QString::fromUtf8(dados);
}

void Serial::on_pbOpera_clicked()
{
    Buf->clear();
    Qsp = new QSerialPort(this);
    Qsp->setBaudRate(ui->cbBaud->currentText().toUInt());
    Qsp->setPortName(ui->cbPort->currentText());
    if(!Qsp->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()
            << "fora";
        return;
    }
    else
        connect(Qsp, &QSerialPort::readyRead, this, &Serial::Lido);
}

Serial::~Serial()
{
    delete ui;
}
