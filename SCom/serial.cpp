#include "serial.h"
#include "ui_serial.h"

Serial::Serial(QWidget *parent): QMainWindow(parent), ui(new Ui::Serial)
{
    ui->setupUi(this);

    m_serial = new QSerialPort(this);
    m_estaGravando = false;
    m_timerTimeout = new QTimer(this);
    m_timerTimeout->setSingleShot(true);
    connect(m_serial, &QSerialPort::readyRead, this, &Serial::onDadosRecebidos);
    connect(m_timerTimeout, &QTimer::timeout, this, &Serial::onTimeoutInatividade);

    ui->cbPari->clear();
    ui->cbPari->addItem("Nenhuma", QSerialPort::NoParity);
    ui->cbPari->addItem("Par", QSerialPort::EvenParity);
    ui->cbPari->addItem("Ímpar", QSerialPort::OddParity);
    ui->cbPari->setToolTip("Sugere-se \"Nenhuma\"");

    ui->cbStop->clear();
    ui->cbStop->addItem("1", QSerialPort::OneStop);
    ui->cbStop->addItem("2", QSerialPort::TwoStop);
    ui->cbStop->setToolTip("Sugere-se \"1\"");

    ui->cbFlow->clear();
    ui->cbFlow->addItem("Nenhum", QSerialPort::NoFlowControl);
    ui->cbFlow->addItem("XON/XOFF (Software)", QSerialPort::SoftwareControl);
    ui->cbFlow->addItem("RTS/CTS (Hardware)", QSerialPort::HardwareControl);
    ui->cbFlow->setToolTip("Sugere-se \"XON/XOFF\" para Leica e \"Nenhum\" para a Geodetic");

    ui->cbDataBit->clear();
    ui->cbDataBit->addItem("7 bits", QSerialPort::Data7);
    ui->cbDataBit->addItem("8 bits", QSerialPort::Data8);
    ui->cbDataBit->setToolTip("Normalmente \"8\"");

    ui->cbBaud->clear();
    ui->cbBaud->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaud->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaud->addItem("38400", QSerialPort::Baud38400);
    ui->cbBaud->addItem("57600", QSerialPort::Baud57600);
    ui->cbBaud->addItem("115200", QSerialPort::Baud115200);
    ui->cbBaud->setToolTip("Sugere-se \"19200\" para Leica e \"9600\" para a Geodetic");

    ui->tbRefrech->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->tbRefrech->setToolTip("Atualizar portas");

    atuPort();
}

void Serial::atuPort()
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
    }
    else
    {
        ui->cbPort->setStyleSheet("");
        ui->cbPort->setToolTip("Selecione a porta da estação total.");
    }
}

void Serial::atuStatus(const QString msg, bool erro)
{
    ui->lbStatus->setText(msg.toUpper());
    if(erro)
        ui->lbStatus->setStyleSheet("color: #ff5252; font-weight: bold;");
    else
        ui->lbStatus->setStyleSheet("color: #4caf50; font-weight: bold;");
}

void Serial::onDadosRecebidos()
{
    m_timerTimeout->start(3000);
    QByteArray dadosNovos = m_serial->readAll();
    if (dadosNovos.isEmpty()) return;
    m_bufferBruto.append(dadosNovos);
    atuLed(1);
    QTimer::singleShot(200, this, [=]() {
        if(m_estaGravando) atuLed(1);
        else atuLed(0);
    });
    QString textoParaExibir = QString::fromLatin1(dadosNovos);
    ui->pteMonitor->insertPlainText(textoParaExibir);
    ui->pteMonitor->ensureCursorVisible();
    ui->lbStatus->setText(QString("RECEBENDO: %1 BYTES").arg(m_bufferBruto.size()));
}

void Serial::salvarArquivoBruto()
{
    if (m_bufferBruto.isEmpty()) return;
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString nomeFinal = QString("%1LEV_%2.gsi").arg(SDIR).arg(timestamp);
    QFile arquivo(nomeFinal);
    if (arquivo.open(QIODevice::WriteOnly)) {
        arquivo.write(m_bufferBruto);
        arquivo.close();

        ui->pteMonitor->appendHtml(QString("<br><b><font color='#ffffff'>ARQUIVO SALVO: %1</font></b>").arg(nomeFinal));
        ui->lbStatus->setText("SALVO COM SUCESSO");
    } else {
        ui->lbStatus->setText("ERRO AO SALVAR ARQUIVO");
    }
    m_bufferBruto.clear();
}

void Serial::onTimeoutInatividade()
{
    // 1. Verificar se realmente temos algo para salvar
    if (m_bufferBruto.isEmpty()) {
        ui->lbStatus->setText("AGUARDANDO DADOS...");
        return;
    }

    // 2. Notificar o usuário no Monitor Matrix
    ui->pteMonitor->appendHtml("<br><font color='#FFA500'>[SISTEMA] Inatividade detectada. Finalizando arquivo...</font>");

    // 3. Chamar a gravação
    salvarArquivoBruto();

    // 4. Resetar o estado da interface (Voltar para IDLE)
    m_estaGravando = false;

    // Fechar a porta (opcional, mas recomendado para liberar o hardware)
    if (m_serial->isOpen()) {
        m_serial->close();
    }

    // Restaurar a aparência do botão e liberar as combos
    ui->pushButton->setText(" INICIAR CAPTURA");
    ui->pushButton->setStyleSheet("background-color: #388E3C; color: white; font-weight: bold;");
    ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    ui->cbPort->setEnabled(true);
    ui->cbBaud->setEnabled(true);

    atuLed(0); // Desliga o LED
    ui->lbStatus->setText("GRAVAÇÃO CONCLUÍDA");
}

void Serial::configurarPorta()
{
    m_serial->setPortName(ui->cbPort->currentData().toString());
    m_serial->setBaudRate(ui->cbBaud->currentData().toInt());
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->cbDataBit->currentData().toInt()));
    //m_serial->setParity(QSerialPort::NoParity);
    m_serial->setParity(static_cast<QSerialPort::Parity>(ui->cbPari->currentData().toInt()));
    //m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->cbStop->currentData().toInt()));
    //m_serial->setFlowControl(QSerialPort::SoftwareControl);
    m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbFlow->currentData().toInt()));
}

void Serial::on_pushButton_clicked()
{
    if (!m_estaGravando)
    {
        configurarPorta();
        if (m_serial->open(QIODevice::ReadOnly))
        {
            m_estaGravando = true;
            m_bufferBruto.clear();
            ui->pteMonitor->clear();
            atualizarInterface(true); // Muda cores para Vermelho/Parar
            ui->lbStatus->setText("OUVINDO ESTAÇÃO...");
            ui->pteMonitor->appendHtml("<font color='#00ff00'>[CONECTADO] Aguardando dados...</font>");
            atuLed(1); // Verde fixo indicando porta aberta
        }
        else
        {
            ui->lbStatus->setText("ERRO: PORTA OCUPADA OU INEXISTENTE");
            atuLed(2); // Vermelho de erro
        }
    }
    else
    {
        m_timerTimeout->stop(); // Cancela o timer pois vamos fechar agora
        onTimeoutInatividade(); // Reutiliza a lógica de salvamento e reset
    }
}

void Serial::atualizarInterface(bool gravando) {
    if (gravando) {
        ui->pushButton->setText(" PARAR E SALVAR");
        ui->pushButton->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));

        ui->cbPort->setEnabled(false);
        ui->cbBaud->setEnabled(false);
        ui->tbRefrech->setEnabled(false);
    } else {
        ui->pushButton->setText(" INICIAR CAPTURA");
        ui->pushButton->setStyleSheet("background-color: #388E3C; color: white; font-weight: bold;");
        ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        ui->cbPort->setEnabled(true);
        ui->cbBaud->setEnabled(true);
        ui->tbRefrech->setEnabled(true);
    }
}
void Serial::atuLed(int estado) {
    /*
       ESTADOS:
       0: Inativo (Cinza)
       1: Ativo/Recebendo (Verde Brilhante)
       2: Erro (Vermelho Alerta)
    */

    QString cor;
    switch (estado) {
    case 1:  cor = "#00FF00"; break; // Verde
    case 2:  cor = "#FF0000"; break; // Vermelho
    default: cor = "#444444"; break; // Cinza
    }

    // Aplicando o estilo para manter o círculo perfeito e a cor desejada
    ui->frame->setStyleSheet(QString(
                                     "QFrame#ledStatus { "
                                     "  background-color: %1; "
                                     "  border: 2px solid #222222; "
                                     "  border-radius: 10px; " // Metade do tamanho 20x20
                                     "}"
                                     ).arg(cor));
}


Serial::~Serial()
{
    delete ui;
}
