#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(serialPort, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    QStringList qsl = {"9600", "19200", "38400", "57600", "115200"};
    ui->cbBaud->addItems(qsl);
    const QList<QSerialPortInfo> qspi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &qsi : qspi)
        ui->cbPort->addItem(qsi.portName());
}

void MainWindow::readData()
{
    Buf += serialPort->readAll();
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        ui->lbStatus->setText("Erro Serial: " + serialPort->errorString());
    }
}

void MainWindow::on_pbAbre_clicked()
{
    serialPort->setBaudRate(ui->cbBaud->currentText().toInt());
    serialPort->setPortName(ui->cbPort->currentText());
    if(!serialPort->open(QIODevice::ReadOnly))
    {
        ui->lbStatus->setText("Erro " + serialPort->errorString());
        return;
    }
    ui->lbStatus->setText("Porta aberta " + ui->cbBaud->currentText() + " bps " + ui->cbPort->currentText());
}

void MainWindow::on_pbFecha_clicked()
{
    if(serialPort->isOpen()) serialPort->close();
    qDebug()
        << Buf;
}

MainWindow::~MainWindow()
{
    delete ui;
}
