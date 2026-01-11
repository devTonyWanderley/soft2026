#include "serial.h"
#include "ui_serial.h"

Serial::Serial(QWidget *parent): QMainWindow(parent), ui(new Ui::Serial)
{
    ui->setupUi(this);
    mSerial = new QSerialPort(this);
    mTimerOut = new QTimer(this);
    mTimerOut->setSingleShot(true);
    mGravando = false;
    connect(mSerial, &QSerialPort::readyRead, this, &Serial::onDdsRec);
    connect(mTimerOut, &QTimer::timeout, this, &Serial::onInativo);
    atuCbFixos();
    atuPorts();
    mSetEstado(mEstado::Desconectado);

    atuInter(false);

    if(ui->cbPort->currentText() == "Nenhuma porta detectada")
        atuStts("AGUARDANDO CONEXÃO FÍSICA", true);
    else
        atuStts("Pronto", false);
}

void Serial::atuCbFixos()
{
    ui->cbPar->clear();
    ui->cbPar->addItem("Nenhuma", QSerialPort::NoParity);
    ui->cbPar->addItem("Par", QSerialPort::EvenParity);
    ui->cbPar->addItem("Ímpar", QSerialPort::OddParity);
    ui->cbPar->setToolTip("Sugere-se \"Nenhuma\"");
    ui->lbPar->setToolTip("Sugere-se \"Nenhuma\"");
    ui->cbStop->clear();
    ui->cbStop->addItem("1", QSerialPort::OneStop);
    ui->cbStop->addItem("2", QSerialPort::TwoStop);
    ui->cbStop->setToolTip("Sugere-se \"1\"");
    ui->lbStop->setToolTip("Sugere-se \"1\"");
    ui->cbFlow->clear();
    ui->cbFlow->addItem("Nenhum", QSerialPort::NoFlowControl);
    ui->cbFlow->addItem("XON/XOFF (Software)", QSerialPort::SoftwareControl);
    ui->cbFlow->addItem("RTS/CTS (Hardware)", QSerialPort::HardwareControl);
    ui->cbFlow->setToolTip("Sugere-se \"XON/XOFF\" para Leica e \"Nenhum\" para a Geodetic");
    ui->lbFlow->setToolTip("Sugere-se \"XON/XOFF\" para Leica e \"Nenhum\" para a Geodetic");
    ui->cbDtBit->clear();
    ui->cbDtBit->addItem("8 bits", QSerialPort::Data8);
    ui->cbDtBit->addItem("7 bits", QSerialPort::Data7);
    ui->cbDtBit->setToolTip("Normalmente \"8\"");
    ui->lbDtBit->setToolTip("Normalmente \"8\"");
    ui->cbBaud->clear();
    ui->cbBaud->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaud->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaud->addItem("38400", QSerialPort::Baud38400);
    ui->cbBaud->addItem("57600", QSerialPort::Baud57600);
    ui->cbBaud->addItem("115200", QSerialPort::Baud115200);
    ui->cbBaud->setToolTip("Sugere-se \"19200\" para Leica e \"9600\" para a Geodetic");
    ui->lbBaud->setToolTip("Sugere-se \"19200\" para Leica e \"9600\" para a Geodetic");
    ui->tbRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->tbRefresh->setToolTip("Atualizar portas");
}

void Serial::atuPorts()
{
    ui->cbPort->clear();
    const auto spi = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &sp : spi)
    {
        QString dpl = sp.portName() + " (" + sp.description() + ")";
        ui->cbPort->addItem(dpl, sp.portName());
    }
    if(ui->cbPort->count() == 0)
    {
        ui->cbPort->addItem("Nenhuma porta detectada");
        ui->cbPort->setStyleSheet(
            "QComboBox { "
            "  background-color: #ffebee; " // Vermelho bem suave (Material Red 50)
            "  border: 1px solid #f44336; " // Borda vermelha para chamar atenção
            "  color: #b71c1c; "            // Texto em vermelho escuro
            "}"
            );
        ui->cbPort->setToolTip("ERRO: Verifique se o cabo USB-Serial está bem conectado.");
        ui->lbPort->setToolTip("ERRO: Verifique se o cabo USB-Serial está bem conectado.");
    }
    else
    {
        ui->cbPort->setStyleSheet("");
        ui->cbPort->setToolTip("Selecione a porta da estação total.");
        ui->lbPort->setToolTip("Selecione a porta da estação total.");
    }
}

void Serial::mSetEstado(mEstado novoEstado)
{
    mEstadoAtual = novoEstado;
    switch (mEstadoAtual) {
    case mEstado::Desconectado:
        mGravando = false;
        mSerial->close();
        mTimerOut->stop();
        ui->pteMoni->appendHtml(QString("<br><b><font color='#ffffff'>INICIAR CAPTURA</font></b>"));
        atuStts("PRONTO", false);
        break;

    case mEstado::Aguardando:
        // ... [Lógica de abertura da porta bem sucedida] ...
        mGravando = true;
        mBuffer.clear();
        // ... [Atualizar interface para LISTENING: Botão Vermelho, Combos Travadas, LED Fixo Verde] ...
        ui->pbMain->setText("PARAR E SALVAR");
        ui->pbMain->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
        ui->cbPort->setEnabled(false);
        ui->cbBaud->setEnabled(false);
        ui->pbMain->setEnabled(false);
        break;

    case mEstado::Recebendo:
        // Este estado é gerenciado principalmente dentro do slot onDadosRecebidos,
        // onde o timer é reiniciado e o LED pisca. A transição para ele ocorre
        // quando o primeiro byte chega em Aguardando.
        mGravando = true;
        break;

    case mEstado::Erro:
        mGravando = false;
        mSerial->close();
        // ... [Atualizar interface para ERRO: LED Vermelho, Label de Status Vermelho] ...
        atuStts("Texto de erro", true);
        ui->pbMain->setEnabled(false); // Trava o botão principal
        break;
    }
    atuInter(mGravando);
}

void Serial::atuStts(const QString msg, bool erro)
{
    ui->lbStatus->setText(msg.toUpper());
    if(erro)
        ui->lbStatus->setStyleSheet("color: #ff5252; font-weight: bold;");
    else
        ui->lbStatus->setStyleSheet("color: #4caf50; font-weight: bold;");
}

void Serial::atuInter(bool gravando)
{
    if (gravando)
    {
        ui->pbMain->setText(" PARAR E SALVAR");
        ui->pbMain->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));

        ui->cbPort->setEnabled(false);
        ui->cbBaud->setEnabled(false);
        ui->pbMain->setEnabled(false);
    }
    else
    {   //  verificar porta e add o terceiro estado
        if(ui->cbPort->currentText() == "Nenhuma porta detectada")
        {
            ui->pbMain->setText(" AGUARDANDO HARD");
            ui->pbMain->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        }
        else
        {
            ui->pbMain->setText(" INICIAR CAPTURA");
            ui->pbMain->setStyleSheet("background-color: #388E3C; color: white; font-weight: bold;");
            ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }

        ui->cbPort->setEnabled(true);
        ui->cbBaud->setEnabled(true);
        ui->tbRefresh->setEnabled(true);
    }
}

void Serial::configPorta()
{
    mSerial->setPortName(ui->cbPort->currentData().toString());
    mSerial->setBaudRate(ui->cbBaud->currentData().toInt());
    mSerial->setDataBits(static_cast<QSerialPort::DataBits>(ui->cbDtBit->currentData().toInt()));
    mSerial->setParity(static_cast<QSerialPort::Parity>(ui->cbPar->currentData().toInt()));
    mSerial->setStopBits(static_cast<QSerialPort::StopBits>(ui->cbStop->currentData().toInt()));
    mSerial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbFlow->currentData().toInt()));
}

void Serial::salva()
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

void Serial::onDdsRec()
{
    mTimerOut->start(3000);
    QByteArray dds = mSerial->readAll();
    if(dds.isEmpty()) return;
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
        ui->lbStatus->setText("AGUARDANDO DADOS...");
        return;
    }
    ui->pteMoni->appendHtml("<br><font color='#FFA500'>[SISTEMA] Inatividade detectada. Finalizando arquivo...</font>");
    salva();
    mGravando = false;
    if(mSerial->isOpen()) mSerial->close();
    ui->pbMain->setText(" INICIAR CAPTURA");
    ui->pbMain->setStyleSheet("background-color: #388E3C; color: white; font-weight: bold;");
    ui->pbMain->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->cbPort->setEnabled(true);
    ui->cbBaud->setEnabled(true);
    ui->lbStatus->setText("GRAVAÇÃO CONCLUÍDA");
}

void Serial::on_tbRefresh_clicked()
{
    atuPorts();
    atuInter(false);
}

/*
void Serial::on_pbMain_clicked()
{
    if (!mGravando)
    {
        configPorta();
        if (mSerial->open(QIODevice::ReadOnly))
        {
            mGravando = true;
            mBuffer.clear();
            ui->pteMoni->clear();
            atuInter(true);
            ui->lbStatus->setText("OUVINDO ESTAÇÃO...");
            ui->pteMoni->appendHtml("<font color='#00ff00'>[CONECTADO] Aguardando dados...</font>");
        }
        else
            ui->lbStatus->setText("ERRO: PORTA OCUPADA OU INEXISTENTE");
    }
    else
    {
        mTimerOut->stop(); // Cancela o timer pois vamos fechar agora
        onInativo(); // Reutiliza a lógica de salvamento e reset
    }
}
*/

void Serial::on_pbMain_clicked()
{
    if (mEstadoAtual == mEstado::Desconectado || mEstadoAtual == mEstado::Erro)
    {
        // Tenta abrir a porta
        configPorta(); // A sua função de ler combos (renomeada)

        if (mSerial->open(QIODevice::ReadOnly))
            mSetEstado(mEstado::Aguardando);
        else
            mSetEstado(mEstado::Erro);

    }
    else if (mEstadoAtual == mEstado::Aguardando || mEstadoAtual == mEstado::Recebendo)
    {
        // Usuário clicou em PARAR/SALVAR
        mTimerOut->stop(); // Cancela o timer para forçar o salvamento
        onInativo();    // Reutiliza a lógica de salvamento e volta para Desconectado
    }
}

Serial::~Serial()
{
    delete ui;
}
