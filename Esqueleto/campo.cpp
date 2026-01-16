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
    connect(cfgS, &QDialog::finished, this, [this](int result) {Campo::opera();});
}

void Campo::opera()
{
    if(*pontFlg)
    {
        ui->lbStatus->setText("CONECTADO COM SUCESSO");
        ui->pbContexto->setText("Iniciar leitura");
        //this->showFullScreen();
        //this->showMaximized();
        this->resize(1200,700);
        QScreen *scren = QGuiApplication::primaryScreen();
        QRect geom = scren->availableGeometry();
        int x = (geom.width() - this->width()) / 2, y = (geom.height()-this->height()) / 2;
        this->move(geom.left() + x, geom.top() + y);
        ui->pteMonitor->setVisible(true);
        ui->cbEdita->setVisible(true);
        ui->pbContexto->setVisible(true);
        ui->qfLed->setStyleSheet("background-color: green; border: 3px solid darked; border-radius: 10px;");
        pontSerial->close();
    }
    else
    {
        ui->lbStatus->setText("NÃO CONECTADO");
        //this->showNormal();
        this->resize(415,145);
        QScreen *scren = QGuiApplication::primaryScreen();
        QRect geom = scren->availableGeometry();
        int x = (geom.width() - this->width()) / 2, y = (geom.height()-this->height()) / 2;
        this->move(geom.left() + x, geom.top() + y);
        ui->pteMonitor->setVisible(false);
        ui->cbEdita->setVisible(false);
        ui->pbContexto->setVisible(false);
        ui->qfLed->setStyleSheet("background-color: red; border: 3px solid darked; border-radius: 10px;");
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
