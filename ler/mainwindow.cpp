#include "mainwindow.h"
#include "ui_mainwindow.h"

pnt::pnt(const QString id, const QString atr, const double x, const double y, const double z)
{
    pNome = id;
    pAtr = atr;
    pAbc = x;
    pOrd = y;
    pCota = z;
}

pnt::pnt(const pnt &outro)
{
    pNome = outro.pNome;
    pAtr = outro.pAtr;
    pAbc = outro.pAbc;
    pOrd = outro.pOrd;
    pCota = outro.pCota;
}

void pnt::pSetId(const QString id)
{
    pNome = id;
}

void pnt::pSetAtr(const QString atr)
{
    pAtr = atr;
}

void pnt::pSetX(const double x)
{
    pAbc = x;
}

void pnt::pSetY(const double y)
{
    pOrd = y;
}

void pnt::pSetZ(const double z)
{
    pCota = z;
}

QString pnt::pGetId()
{
    return pNome;
}

QString pnt::pGetAtr()
{
    return pAtr;
}

double pnt::pGetX()
{
    return pAbc;
}

double pnt::pGetY()
{
    return pOrd;
}

double pnt::pGetZ()
{
    return pCota;
}

bool Importa(QByteArray &buf, const QString dir, const QString filtro, const QString titulo)
{
    QString id = QFileDialog::getOpenFileName(nullptr, titulo, dir, filtro);
    if(id == "") return false;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::ReadOnly))return false;
    buf = arq.readAll();
    arq.close();
    return true;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cfg = new cfgCom(this);
    cfg->show();
    QString id = QFileDialog::getOpenFileName(nullptr, "abrir", "C:\\Tony\\Soft\\soft2026\\Inst\\", "Arquivo dxf (*.dxf)");
    if(id == "") return;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::ReadOnly)) return;
    QTextStream tubo(&arq);
    QString titulo, valor, v1;
    titulo = valor = v1 = "";
    bool dentro = false;
    //QByteArray titulo, valor;
    while(!tubo.atEnd())
    {
        if(valor == "" || valor.indexOf('\r') == -1)
            titulo = tubo.readLine().trimmed();
        else
            titulo = valor.right(valor.length() - valor.indexOf('\r') - 1);
        if(titulo.indexOf('\r') == -1)
            valor = tubo.readLine().trimmed();
        else
            valor = titulo.right(titulo.length() - titulo.indexOf('\r') - 1);
        if(valor.indexOf('\r') == -1)
            v1 = valor;
        else
            v1 = valor.left(valor.indexOf('\r'));
        if(dentro)
        {
            if(titulo == " 0" && v1 == "ENDSEC")
                dentro = false;
        }
        /*
        else
        {
            if(titulo == "2" && v1 == "ENTITIES")
                dentro = true;
        }
        */
        if(dentro)
            qDebug() << titulo << '\t' << v1;
        if(!dentro)
        {
            if(titulo == "2" && v1 == "ENTITIES")
                dentro = true;
        }
    }
    arq.close();
    /*
    if(Importa(lBuffer))
    {
        qDebug()
            << lBuffer;
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
