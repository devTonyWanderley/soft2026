#include "cfgcom.h"
#include "ui_cfgcom.h"

cfgCom::cfgCom(QWidget *parent): QDialog(parent), ui(new Ui::cfgCom)
{
    ui->setupUi(this);
    flg = false;
    ui->pbOk->setText("CONECTAR");
    iniCombos();
    atuPorts();
}

void cfgCom::iniCombos()
{
    ui->cbPar->addItem("Nenhuma", QSerialPort::NoParity);
    ui->cbPar->addItem("Par", QSerialPort::EvenParity);
    ui->cbPar->addItem("Ímpar", QSerialPort::OddParity);
    ui->cbStop->addItem("1", QSerialPort::OneStop);
    ui->cbStop->addItem("2", QSerialPort::TwoStop);
    ui->cbFlow->addItem("Nenhum", QSerialPort::NoFlowControl);
    ui->cbFlow->addItem("XON/XOFF (Software)", QSerialPort::SoftwareControl);
    ui->cbFlow->addItem("RTS/CTS (Hardware)", QSerialPort::HardwareControl);
    ui->cbData->addItem("8 bits", QSerialPort::Data8);
    ui->cbData->addItem("7 bits", QSerialPort::Data7);
    ui->cbBaud->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaud->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaud->addItem("38400", QSerialPort::Baud38400);
    ui->cbBaud->addItem("57600", QSerialPort::Baud57600);
    ui->cbBaud->addItem("115200", QSerialPort::Baud115200);
}

bool cfgCom::atuPorts()
{
    ui->cbPort->clear();
    const auto spi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &sp : spi)
    {
        QString dpl = sp.portName() + " (" + sp.description() + ")";
        ui->cbPort->addItem(dpl, sp.portName());
    }
    if(ui->cbPort->count()) return true;
    return false;
}

void cfgCom::on_pbOk_clicked()
{
    if(!flg)
    {
        cSerial.setPortName(ui->cbPort->currentData().toString());
        cSerial.setBaudRate(ui->cbBaud->currentData().toInt());
        cSerial.setDataBits(static_cast<QSerialPort::DataBits>(ui->cbData->currentData().toInt()));
        cSerial.setParity(static_cast<QSerialPort::Parity>(ui->cbPar->currentData().toInt()));
        cSerial.setStopBits(static_cast<QSerialPort::StopBits>(ui->cbStop->currentData().toInt()));
        cSerial.setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbFlow->currentData().toInt()));
        if(cSerial.open(QIODevice::ReadOnly))
        {
            flg = true;
            ui->pbCancela->setVisible(true);
            ui->pbOk->setText("OK");
            ui->frame->setStyleSheet("background-color: green; border: 3px solid darked; border-radius: 10px;");
        }
        return;
    }
    this->close();
}

void cfgCom::on_pbCancela_clicked()
{
    flg = false;
    if(cSerial.isOpen()) cSerial.close();
    this->close();
}

cfgCom::~cfgCom()
{
    delete ui;
}
