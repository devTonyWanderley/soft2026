#include "civil.h"
#include "ui_civil.h"

Civil::Civil(QWidget *parent): QMainWindow(parent), ui(new Ui::Civil)
{
    ui->setupUi(this);
}

void Civil::on_actionLeica_triggered()
{
    qDebug()
        << "Serial leica";
    cs = new Serial;
    cs->setWindowTitle("Leica");
    cs->Buf = &Buffer;
    cs->show();
}

void Civil::on_actionGD5_triggered()
{
    qDebug()
        << "Serial GD5";
    cs = new Serial;
    cs->setWindowTitle("Geodetic GD5");
    cs->Buf = &Buffer;
    cs->show();
}

void Civil::on_actionDxf_leica_triggered()
{
    qDebug()
        << "Dxf leica";
}

void Civil::on_actionPnt_triggered()
{
    qDebug()
        << Buffer
        << " Pnt";
}

void Civil::on_actionArs_triggered()
{
    qDebug()
        << "Ars";
}

void Civil::on_actionSup_triggered()
{
    qDebug()
        << "Sup";
}

void Civil::on_actionTrh_triggered()
{
    qDebug()
        << "Trh = traçado horizontal";
}

Civil::~Civil()
{
    delete ui;
}
