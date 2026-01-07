#include "leica.h"
#include "ui_leica.h"

Leica::Leica(QWidget *parent): QMainWindow(parent), ui(new Ui::Leica)
{
    ui->setupUi(this);
    QStringList qsl = {"9600", "19200", "38400", "57600", "115200"};
    ui->cbBaud->addItems(qsl);
    const QList<QSerialPortInfo> qspi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &qsi : qspi)
        ui->cbPort->addItem(qsi.portName());
    Buffer.clear();
    Passo = 0;
    connect(&Qsp, &QSerialPort::readyRead, this, &Leica::LerSerial);
    connect(&Qsp, &QSerialPort::errorOccurred, this, &Leica::TrataErro);
}

void Leica::LerSerial()
{
    Buffer += Qsp.readAll();
}

void Leica::TrataErro(QSerialPort::SerialPortError e)
{
    if(e != QSerialPort::NoError)
    {
        ui->lbStatus->setText("Erro Serial: " + Qsp.errorString());
        Passo = 0;
    }
}

void Leica::on_pbOp_clicked()
{
    if(!Passo)
    {
        Qsp.setBaudRate(ui->cbBaud->currentText().toUInt());
        Qsp.setPortName(ui->cbPort->currentText());
        if(!Qsp.open(QIODevice::ReadOnly))
        {
            ui->lbStatus->setText("Erro " + Qsp.errorString());
            return;
        }
        ui->pbOp->setText("Salvar dxf");
        ui->lbStatus->setText("Lendo..");
        Passo++;
        return;
    }
    if(Passo == 1)
    {
        QString buf = Buffer;
        if(!Exporta(buf, "DXF (*.dxf)", "Salvar dxf")) return;
        ui->pbOp->setText("Salvar pnt");
        ui->lbStatus->setText("DXF");
        Passo++;
        return;
    }
    if(Passo == 2)
    {
        QString buf = Buffer, ln, bu = "";
        while(LerPntDXl(buf, ln))
        {
            if(bu.length()) bu += '\n';
            bu += ln;
        }
        if(!Exporta(bu, "PNT (*.pnt)", "Salvar pnt")) return;
        ui->pbOp->setText("Outra leitura...");
        ui->lbStatus->setText("PNT");
        Passo = 0;
        return;
    }
}

Leica::~Leica()
{
    if(Qsp.isOpen()) Qsp.close();
    delete ui;
}
