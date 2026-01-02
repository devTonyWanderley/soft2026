#include "civil.h"
#include "ui_civil.h"

Civil::Civil(QWidget *parent): QMainWindow(parent), ui(new Ui::Civil)
{
    ui->setupUi(this);
}

void Civil::on_actionLeica_triggered()
{
    qDebug()
        << "Importar serial leica .. a implementar";
    cs = new ComSerial(this);
    cs->setWindowTitle("Comunicação serial com estação Leica");
    cs->Buf = &QbaBuf;
    cs->show();
}

void Civil::on_actionGD5_triggered()
{
    qDebug()
        << "Importar serial GD5 .. a implementar";
    cs = new ComSerial(this);
    cs->setWindowTitle("Comunicação serial com estação Geodetic GD5");
    cs->Buf = &QbaBuf;
    cs->show();
}

void Civil::on_actionDxf_leica_triggered()
{
    qDebug()
        << "Importar dxf leica .. a implementar";
}

void Civil::on_actionPnt_triggered()
{
    qDebug()
        << "Importar pnt .. a implementar";
    qDebug()
        << QbaBuf;
}

void Civil::on_actionArs_triggered()
{
    qDebug()
        << "Importar ars .. a implementar";
}

void Civil::on_actionSup_triggered()
{
    qDebug()
        << "Importar sup .. a implementar";
}

void Civil::on_actionTrh_triggered()
{
    qDebug()
        << "Importar trh = traçado horizontal .. a implementar";
}

Civil::~Civil()
{
    delete ui;
}
