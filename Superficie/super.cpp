#include "super.h"
#include "ui_super.h"

Super::Super(QWidget *parent): QMainWindow(parent), ui(new Ui::Super)
{
    ui->setupUi(this);
    passo = 0;
    ui->pbOp->setText("Importar pontos");
}

void Super::on_pbOp_clicked()
{
    if(!passo)
    {
        if(!Importa(Buffer, "PNT (*.pnt)", "Adquirir pnt")) return;
        QString tx;
        while(Buffer.length() > 45)
        {
            tx = Buffer.left(45);
            Buffer = Buffer.right(Buffer.length() - 46);
            sup.addPonto(tx);
        }
        sup.addPonto(Buffer);
        Buffer.clear();
        passo++;
        ui->pbOp->setText("Importar arestas");
        qDebug() << sup.numPontos();    //                  lembrar de apagar
        return;
    }
    if(passo == 1)
    {
        if(!Importa(Buffer, "PNT (*.ars)", "Adquirir ars")) return;
        QString tx;
        while(Buffer.length() > 16)
        {
            tx = Buffer.left(16);
            Buffer = Buffer.right(Buffer.length() - 17);
            sup.addAresta(tx);
        }
        sup.addAresta(Buffer);
        Buffer.clear();
        passo++;
        ui->pbOp->setText("Gerar superfície");
        qDebug() << sup.numArestas();    //                  lembrar de apagar
        return;
    }
    if(passo == 2)
    {
        sup.geraFaces();
        qDebug() << sup.numFaces();
    }
}

Super::~Super()
{
    delete ui;
}
