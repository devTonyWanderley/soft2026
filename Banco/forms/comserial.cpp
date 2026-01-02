#include "comserial.h"
#include "ui_comserial.h"

ComSerial::ComSerial(QWidget *parent): QDialog(parent), ui(new Ui::ComSerial)
{
    ui->setupUi(this);
    QStringList qsl = {"9600", "19200", "38400", "57600", "115200"};
    ui->cbBaud->addItems(qsl);
    const QList<QSerialPortInfo> qspi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &qsi : qspi)
        ui->cbPort->addItem(qsi.portName());
    ui->pbAbre->setEnabled(true);
    ui->pbGuarda->setEnabled(false);
    ui->lbStatus->setText("Aguardando   ...  .. .");
    Buf = new QByteArray;
    Buf->clear();

    Qsp = new QSerialPort(this);
    connect(Qsp, &QSerialPort::readyRead, this, &ComSerial::Ler);
    connect(Qsp, &QSerialPort::errorOccurred, this, &ComSerial::Erro);
}

void ComSerial::Ler()
{
    *Buf += Qsp->readAll();
}

void ComSerial::Erro(QSerialPort::SerialPortError e)
{
    if(e != QSerialPort::NoError)
        ui->lbStatus->setText("Erro Serial: " + Qsp->errorString());
}

void ComSerial::on_pbAbre_clicked()
{
    Qsp->setBaudRate(ui->cbBaud->currentText().toUInt());
    Qsp->setPortName(ui->cbPort->currentText());
    if(!Qsp->open(QIODevice::ReadOnly))
    {
        ui->lbStatus->setText("Erro " + Qsp->errorString());
        return;
    }
    ui->pbAbre->setEnabled(false);
    ui->pbGuarda->setEnabled(true);
    ui->lbStatus->setText("Porta aberta " + ui->cbBaud->currentText() + " bps " + ui->cbPort->currentText());
}

ComSerial::~ComSerial()
{
    delete ui;
}
