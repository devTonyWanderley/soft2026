#include "serial.h"
#include "ui_serial.h"

//      --FUNÇÕES--
Serial::Serial(QWidget *parent): QMainWindow(parent), ui(new Ui::Serial)
{
    //  instanciar:
    ui->setupUi(this);
    mIniciado = false;
    mSerial = new QSerialPort(this);
    mTimer = new QTimer(this);

    //  configurar:
    mTimer->setSingleShot(true);
    connect(mSerial, &QSerialPort::readyRead, this, &Serial::onSDados);
    connect(mTimer, &QTimer::timeout, this, &Serial::onInativo);
    mIniDlg();  //  Carregar combos
    if(mAtuPorts())
        mSetEstado(mEstado::Desconectado);
    else
        mSetEstado(mEstado::Erro);
    mIniciado = true;
}

void Serial::mIniDlg()  //  Inicia os comboboxes e atribui o ícone de tbRefresh
{
    ui->cbPar->addItem("Nenhuma", QSerialPort::NoParity);
    ui->cbPar->addItem("Par", QSerialPort::EvenParity);
    ui->cbPar->addItem("Ímpar", QSerialPort::OddParity);
    ui->cbStop->addItem("1", QSerialPort::OneStop);
    ui->cbStop->addItem("2", QSerialPort::TwoStop);
    ui->cbFlow->addItem("Nenhum", QSerialPort::NoFlowControl);
    ui->cbFlow->addItem("XON/XOFF (Software)", QSerialPort::SoftwareControl);
    ui->cbFlow->addItem("RTS/CTS (Hardware)", QSerialPort::HardwareControl);
    ui->cbDtBit->addItem("8 bits", QSerialPort::Data8);
    ui->cbDtBit->addItem("7 bits", QSerialPort::Data7);
    ui->cbBaud->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaud->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaud->addItem("38400", QSerialPort::Baud38400);
    ui->cbBaud->addItem("57600", QSerialPort::Baud57600);
    ui->cbBaud->addItem("115200", QSerialPort::Baud115200);
    ui->tbRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

bool Serial::mAtuPorts()    //  Atualiza cbPort
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

void Serial::mSetCombos(bool f) //  Habilita / desabilita comboboxes, conforme 'f'
{
    ui->cbBaud->setEnabled(f);
    ui->cbDtBit->setEnabled(f);
    ui->cbFlow->setEnabled(f);
    ui->cbPar->setEnabled(f);
    ui->cbPort->setEnabled(f);
    ui->cbStop->setEnabled(f);
}

void Serial::mCfgPorta()    //  Configura porta serial com os valore escolhidos
{
    mSerial->setPortName(ui->cbPort->currentData().toString());
    mSerial->setBaudRate(ui->cbBaud->currentData().toInt());
    mSerial->setDataBits(static_cast<QSerialPort::DataBits>(ui->cbDtBit->currentData().toInt()));
    mSerial->setParity(static_cast<QSerialPort::Parity>(ui->cbPar->currentData().toInt()));
    mSerial->setStopBits(static_cast<QSerialPort::StopBits>(ui->cbStop->currentData().toInt()));
    mSerial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbFlow->currentData().toInt()));
}

void Serial::mExporta() //  Salva o arquivo bruto (*.bru)
{
    if(mBuffer.isEmpty()) return;
    QString tmpTx = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString nome = QString("%1LEV_%2.bru").arg(SDIR).arg(tmpTx);
    QFile arquivo(nome);
    if (arquivo.open(QIODevice::WriteOnly))
    {
        arquivo.write(mBuffer);
        arquivo.close();
        ui->pteMoni->appendHtml(QString("<br><b><font color='#ffffff'>ARQUIVO SALVO: %1</font></b>").arg(nome));
        ui->lbStatus->setText("SALVO COM SUCESSO");
    }
    else
        ui->lbStatus->setText("ERRO AO SALVAR ARQUIVO");
    mBuffer.clear();
}

void Serial::mSetEstado(mEstado estado) //  Gerencia fluxo
{
    mEAtu = estado;
    switch (mEAtu)
    {
    case mEstado::Desconectado:
        mSetCombos();
        ui->lbStatus->setStyleSheet("color: #4caf50; font-weight: bold;");
        ui->lbStatus->setText(" --PRONTO--");
        ui->pbMain->setStyleSheet("color: #4caf50; font-weight: bold;");
        ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
        ui->pbMain->setText("--CONECTAR--");
        ui->cbPort->setStyleSheet("");
        if(mSerial->isOpen()) mSerial->close();
        mTimer->stop();
        ui->pteMoni->appendHtml(QString("<br><b><font color='#ffffff'>INICIAR CAPTURA</font></b>"));
        break;
    case mEstado::Aguardando:
        //  [todos os combos com aparencia normal e desabilitados]
        //  [pbMain "Desconectar"]
        mSetCombos(false);
        ui->lbStatus->setStyleSheet("color: #4cae50; font-weight: bold;");
        ui->lbStatus->setText(" --AGUARDANDO DADOS--");
        ui->pbMain->setText("--DESCONECTAR--");
        ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
        break;
    case mEstado::Recebendo:
        qDebug()
            << "Recebendo";
        break;
    default:
        //  [mudar cbPort: *ALERTA DE ERRO*, "PORTA INVÁLIDA"]
        qDebug()
            << "Por enquanto, usa-se default como o estado \"ERRO\"";
        break;
    }
}

//      --SLOTS--
void Serial::onSDados()    //  Recebe os dados
{
    mTimer->start(3000);
    QByteArray dds = mSerial->readAll();
    if(dds.isEmpty()) return;
    if(mBuffer.isEmpty())
    {
        ui->pbMain->setText(" PARAR E SALVAR");
        ui->pbMain->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    }
    mBuffer.append(dds);
    QString tx = QString::fromLatin1(dds);
    ui->pteMoni->insertPlainText(tx);
    ui->pteMoni->ensureCursorVisible();
    ui->lbStatus->setText(QString("RECEBENDO: %1 BYTES").arg(mBuffer.size()));
}

void Serial::onInativo()
{
    if(mBuffer.isEmpty())
    {
        ui->lbStatus->setText("--AGUARDANDO DADOS--");
        return;
    }
    ui->pteMoni->appendHtml("<br><font color='#FFA500'>[SISTEMA] Inatividade detectada. Finalizando arquivo...</font>");
    mExporta();
    mSetEstado(mEstado::Desconectado);
}

void Serial::on_pbMain_clicked()
{
    if(mEAtu == mEstado::Desconectado)
    {
        mCfgPorta();
        if(mSerial->open(QIODevice::ReadOnly))
            mSetEstado(mEstado::Aguardando);
        else
            mSetEstado(mEstado::Erro);
        return;
    }
    if(mEAtu == mEstado::Aguardando)
    {
        mSetEstado(mEstado::Desconectado);
        return;
    }
    if(mEAtu == mEstado::Recebendo)
    {
        if(!mBuffer.isEmpty())
            mExporta();
        mSetEstado(mEstado::Desconectado);
        return;
    }
}

void Serial::on_tbRefresh_clicked()
{
    qDebug()
        << "tbRefrech .. atualiza";
}

void Serial::on_cbPort_currentIndexChanged(int index)
{}
void Serial::on_cbBaud_currentIndexChanged(int index)
{}
void Serial::on_cbDtBit_currentIndexChanged(int index)
{}
void Serial::on_cbPar_currentIndexChanged(int index)
{}
void Serial::on_cbStop_currentIndexChanged(int index)
{}
void Serial::on_cbFlow_currentIndexChanged(int index)
{}

Serial::~Serial()
{
    delete ui;
}
