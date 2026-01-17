#include "campo.h"
#include "ui_campo.h"

Campo::Campo(QWidget *parent): QMainWindow(parent), ui(new Ui::Campo)
{
    ui->setupUi(this);
    ui->pteMonitor->setReadOnly(true);
    ui->pteMonitor->setVisible(false);
    ui->cbEdita->setVisible(false);
    ui->pbContexto->setVisible(false);
    cfgS = new cfgCom(this);
    pontSerial = &cfgS->cSerial;
    pontFlg = &cfgS->flg;
    timer =  new QTimer(this);
    connect(cfgS, &QDialog::finished, this, [this](int result) {Campo::opera();});
    timer->setSingleShot(true);
    connect(pontSerial, &QSerialPort::readyRead, this, &Campo::onSDados);
    connect(timer, &QTimer::timeout, this, &Campo::onInativo);
}

void Campo::redim(int b, int h)
{
    this->resize(b, h);
    QRect geom = QGuiApplication::primaryScreen()->availableGeometry();
    int x = (geom.width() - this->width()) / 2, y = (geom.height()-this->height()) / 2;
    this->move(geom.left() + x, geom.top() + y);
}

void Campo::onSDados()
{
    timer->start(7000);
    QByteArray dds = pontSerial->readAll();
    if(dds.isEmpty()) return;
    if(buffer.isEmpty())
    {
        ui->pbContexto->setText(" PARAR E SALVAR");
        ui->pbContexto->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold;");
        ui->pbContexto->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    }
    buffer.append(dds);
    QString tx = QString::fromLatin1(dds);
    ui->pteMonitor->insertPlainText(tx);
    ui->pteMonitor->ensureCursorVisible();
    ui->lbStatus->setText(QString("RECEBENDO: %1 BYTES").arg(buffer.size()));
}

void Campo::exporta()
{
    if(buffer.isEmpty()) return;
    QString tmpTx = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString nome = QString("%1LEV_%2.bak").arg(SDIR).arg(tmpTx);
    QFile arquivo(nome);
    if (arquivo.open(QIODevice::WriteOnly))
    {
        arquivo.write(buffer);
        arquivo.close();
        ui->pteMonitor->appendHtml(QString("<br><b><font color='#ffffff'>ARQUIVO SALVO: %1</font></b>").arg(nome));
        ui->lbStatus->setText("SALVO COM SUCESSO");
    }
    else
        ui->lbStatus->setText("ERRO AO SALVAR ARQUIVO");
    buffer.clear();
}

void Campo::onInativo()
{
    if(buffer.isEmpty())
    {
        ui->lbStatus->setText("--AGUARDANDO DADOS--");
        return;
    }
    ui->pteMonitor->appendHtml("<br><font color='#FFA500'>[SISTEMA] Inatividade detectada. Finalizando arquivo...</font>");
    exporta();
    //mSetEstado(mEstado::Desconectado);
}

void Campo::opera()
{
    if(*pontFlg)
    {
        ui->lbStatus->setText("CONECTADO COM SUCESSO");
        ui->pbContexto->setText("Iniciar leitura");
        redim(1200,700);
        ui->pteMonitor->setVisible(true);
        ui->cbEdita->setVisible(true);
        ui->pbContexto->setVisible(true);
        ui->qfLed->setStyleSheet("background-color: lime; border: 2px solid darked; border-radius: 10px;");
        //pontSerial->close();    //  aqui se inicia a leitura    ..  copiar lógica de "WSerial"
    }
    else
    {
        ui->lbStatus->setText("NÃO CONECTADO");
        redim();
        ui->pteMonitor->setVisible(false);
        ui->cbEdita->setVisible(false);
        ui->pbContexto->setVisible(false);
        ui->qfLed->setStyleSheet("background-color: #ff0000; border: 3px solid darked; border-radius: 10px;");
    }
}

void Campo::on_cbEdita_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1) ui->pteMonitor->setReadOnly(false);
    else ui->pteMonitor->setReadOnly(true);
}

void Campo::on_pbCfg_clicked()
{
    cfgS->exec();
}

Campo::~Campo()
{
    delete ui;
}
