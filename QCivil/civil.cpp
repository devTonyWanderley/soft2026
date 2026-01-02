#include "civil.h"
#include "ui_civil.h"

Civil::Civil(QWidget *parent): QMainWindow(parent), ui(new Ui::Civil)
{
    ui->setupUi(this);
}

void Civil::GuardaPontos()
{
    if(!Buffer.length()) return;
    Lp.Clear();
    while(Buffer.length() > 45)
    {
        qDebug()
            << Buffer.left(45)             //  //  //  //
            << Buffer.right(Buffer.length() - 46).left(45);     //  //  //  //  //
        Lp.Pushback(Ponto(Buffer.left(45)));
        Buffer = Buffer.right(Buffer.length() - 46);
    }
    qDebug()
        << Lp.Length()
        << " pontos lidos";
}

void Civil::GuardaArestas()
{
    if(!Buffer.length()) return;
    La.Clear();
    while(Buffer.length() > 16)
    {
        La.Pushback(Aresta(Buffer.left(16), Lp.GetPrimo()));
        Buffer = Buffer.right(Buffer.length() - 17);
    }
    qDebug()
        << La.Length()
        << " arestas lidas";
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
        << "Importar dxf leica .. implementando";
    if(!Importa(Buffer, "dxf (*.dxf)", "Ler pontos do dxf")) return;
    if(!SeparaEntities(Buffer)) return;
    QString id, atr, x, y, z, buf = "";
    while(LerProxPonto(Buffer, id, atr, x, y, z))
    {
        if(buf.length()) buf += '\n';
        buf += (id + atr + x + y + z);
    }
    Buffer = buf;
    qDebug()
        << "Buffer tem "
        << Buffer.length()
        << " caracteres .. \nfalta guardar numa lista de pontos";
}

void Civil::on_actionPnt_triggered()
{
    qDebug()
        << "Importar pnt .. implementando";
    if(!Importa(Buffer, "Pontos (*.pnt)", "Ler pontos pnt")) return;
    qDebug()
        << "Buffer tem "
        << Buffer.length()
        << " caracteres";
    GuardaPontos();
}

void Civil::on_actionArs_triggered()
{
    qDebug()
        << "Importar ars .. implementando";
    if(!Importa(Buffer, "Arestas (*.ars)", "Ler arestas ars")) return;
    qDebug()
        << "Buffer tem "
        << Buffer.length()
        << " caracteres .. \nfalta guardar numa lista de arestas";
}

void Civil::on_actionSup_triggered()
{
    qDebug()
        << "Importar sup .. implementando";
    if(!Importa(Buffer, "Superfície (*.sup)", "Ler Superfície sup")) return;
    qDebug()
        << "Buffer tem "
        << Buffer.length()
        << " caracteres .. \nfalta guardar numa lista de faces";
}

void Civil::on_actionTrh_triggered()
{
    qDebug()
        << "Importar trh = traçado horizontal .. a implementar";
}

void Civil::on_actionPnt_2_triggered()
{
    qDebug()
        << "Exportar pnt .. a implementar";
}

void Civil::on_actionSup_2_triggered()
{
    qDebug()
        << "Exportar sup .. a implementar";
}

void Civil::on_actionCon_triggered()
{
    qDebug()
        << "Exportar pnt .. a implementar";
}

Civil::~Civil()
{
    delete ui;
}
