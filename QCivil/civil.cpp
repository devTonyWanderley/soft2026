#include "civil.h"
#include "ui_civil.h"

Civil::Civil(QWidget *parent): QMainWindow(parent), ui(new Ui::Civil)
{
    ui->setupUi(this);
}

void Civil::on_actionLeica_triggered()
{
    qDebug()
        << "Leitura direto da estação leica";
}

void Civil::on_actionGD5_triggered()
{
    qDebug()
        << "Leitura direto da estação geodetic";
}

void Civil::on_actionPnt_triggered()
{
    qDebug()
        << "Leitura de arquivo de pontos";
}

void Civil::on_actionArs_triggered()
{
    qDebug()
        << "Leitura de arquivo de arestas";
}

void Civil::on_actionSup_triggered()
{
    qDebug()
        << "Leitura de arquivo de superfície .. "
        << pri.GNome();
    pri.SNome("Primitiva");
    qDebug()
        << "Agora o nome é:\t"
        << pri.GNome();
}

void Civil::on_actionTrh_triggered()
{
    qDebug()
        << "Leitura de arquivo de traçado horizontal";
}

Civil::~Civil()
{
    delete ui;
}
