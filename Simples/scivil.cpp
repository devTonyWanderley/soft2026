#include "scivil.h"
#include "ui_scivil.h"

SCivil::SCivil(QWidget *parent): QMainWindow(parent), ui(new Ui::SCivil)
{
    ui->setupUi(this);
    cs = new ComSerial;
    cs->Buf = &Buffer;
    cs->Flg = &Passo;
}

void SCivil::on_pbSerial_clicked()
{
    ui->pbGera->setText("Salvar dxf");
    Passo = 0;
    cs->show();
}

void SCivil::on_pbDxf_clicked()
{
    qDebug()
    << "Importar dxf";
    if(!Importa(Buffer, "Arquivo dxf (*.dxf)", "Importar arquivo leica dxf"))
    {
        qDebug()
            << "função cancelada";
        return;
    }
    QString buf = Buffer, ln;
    while(LerPntDXl(buf, ln))
        qDebug()
            << ln;
}

void SCivil::on_pbIpnt_clicked()
{
    qDebug()
    << "Importar pontos";
}

void SCivil::on_pbArs_clicked()
{
    qDebug()
    << "Importar arestas";
}

void SCivil::on_pbEpnt_clicked()
{
    qDebug()
    << "Exportar pontos";
}

void SCivil::on_pbFce_clicked()
{
    qDebug()
    << "Exportar faces";
}

void SCivil::on_pbCon_clicked()
{
    qDebug()
    << "Exportar contorno";
}

void SCivil::on_pbCn_clicked()
{
    qDebug()
        << "Exportar curvas de nível";
}

void SCivil::on_pbGera_clicked()
{
    if(!Passo)
    {
        qDebug()
            << "Função cancelada";
    }
    if(Passo == 1)
    {
        qDebug()
            << Buffer;
        return;
    }
}

SCivil::~SCivil()
{
    delete ui;
}
