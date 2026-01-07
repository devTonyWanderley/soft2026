#include "scivil.h"
#include "ui_scivil.h"

SCivil::SCivil(QWidget *parent): QMainWindow(parent), ui(new Ui::SCivil)
{
    ui->setupUi(this);
    cs = new ComSerial;
    cs->Buf = &Buffer;
    cs->Flg = &Passo;
}

void SCivil::CarregaPontos(QString buf)
{
    QString tx;
    Lp.Clear();
    while(buf.length() > 45)
    {
        tx = buf.left(45);
        buf = buf.right(buf.length() - 46);
        Lp.Pushback(Ponto(tx));
    }
    Lp.Pushback(Ponto(buf));
    qDebug()
        << Lp.Length();
}

void SCivil::CarregaArestas(QString buf)
{
    QString tx;
    La.Clear();
    while(buf.length() > 16)
    {
        tx = buf.left(16);
        buf = buf.right(buf.length() - 17);
        La.Pushback(Aresta(tx, Lp.GetPrimo()));
    }
    //La.Pushback(buf, Lp.GetPrimo());
    qDebug()
        << La.Length();
}

void SCivil::on_pbSerial_clicked()
{
    ui->pbGera->setText("Salvar dxf");
    Passo = 0;
    cs->show();
}

void SCivil::on_pbDxf_clicked()
{
    if(!Importa(Buffer, "Arquivo dxf (*.dxf)", "Importar arquivo leica dxf"))
    {
        qDebug()
            << "função cancelada";
        return;
    }
    ui->pbGera->setText("Salvar pnt");
    QString buf = Buffer, ln, bu = "";
    while(LerPntDXl(buf, ln))
    {
        if(bu.length()) bu += '\n';
        bu += ln;
    }
    Passo = 2;
    Buffer = bu;
}

void SCivil::on_pbIpnt_clicked()
{
    qDebug()
    << "Importar pontos";
    if(!Importa(Buffer, "Pontos (*.pnt)", "----------")) return;
    CarregaPontos(Buffer);
}

void SCivil::on_pbArs_clicked()
{
    if(!Importa(Buffer, "Arestas (*.ars)", "----------")) return;
    if(!Lp.Length()) return;
    CarregaArestas(Buffer);
    Passo = 3;
    ui->pbGera->setText("Gerar faces");
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
            << "Salvar o obtido na porta serial ..  aqui começa:";
        ui->pbGera->setText("Salvar pnt");
        QString buf = Buffer, ln, bu = "";
        while(LerPntDXl(buf, ln))
        {
            if(bu.length()) bu += '\n';
            bu += ln;
        }
        Buffer = bu;
        if(!Exporta(Buffer, "Pontos (*.pnt)", "_________")) return;
        CarregaPontos(Buffer);
        return;
    }
    if(Passo == 2)
    {
        if(!Exporta(Buffer, "Pontos (*.pnt)", "_________")) return;
        CarregaPontos(Buffer);
        return;
    }
}

SCivil::~SCivil()
{
    delete ui;
}
