#include "trhor.h"
#include "ui_trhor.h"
#include <QFile>
#include <QFileDialog>
#include <QPointF>
#include <QLineF>

Ponto::Ponto(const QString id,
             const QString atr,
             const double x,
             const double y,
             const double z)
{
    nome = id;
    atri = atr;
    abci = x;
    orde = y;
    cota = z;
}

Aresta::Aresta(const QString v0, const QString v1)
{
    vIni = v0;
    vFim = v1;
}

Face::Face(const QString aa, const QString bb, const QString cc)
{
    va = aa;
    vb = bb;
    vc = cc;
}

pnTrh::pnTrh(const int s,
             const double x, const double y, const QChar tp)
{
    serie = s;
    X = x;
    Y = y;
    tipo = tp;
}

trHor::trHor(QWidget *parent): QMainWindow(parent), ui(new Ui::trHor)
{
    ui->setupUi(this);
    if(!parseArestas())
    {
        qDebug()
            << "Problemas com arestas";
        return;
    }
    qDebug()
        << arestas.length()
        << " arestas lidas";
    if(!parsePontos())
    {
        qDebug()
        << "Problemas com pontos";
        return;
    }
    qDebug()
        << pontos.length()
        << " pontos lidos";
    if(!parseTrhs())
    {
        qDebug()
        << "Problemas com pontos";
        return;
    }
    qDebug()
        << pntrhs.length()
        << " pontos notáveis lidos";
    geraFaces();
    qDebug()
        << faces.length()
        << " faces";
}

bool trHor::Importa(QString titulo, QString dir, QString filtro)
{
    QString id = QFileDialog::getOpenFileName(this, titulo, dir, filtro);
    if(id.isEmpty()) return false;
    QFile arq(id);
    if(!arq.open(QIODevice::ReadOnly)) return false;
    buffer.clear();
    buffer = arq.readAll();
    arq.close();
    return true;
}

bool trHor::parseArestas()
{
    if(!Importa("Ler ARESTAS", "C:\\2026\\Soft\\Instâncias\\", "arquivo de arestas (*.ars)")) return false;
    int b = 0;
    QString p, q;
    arestas.clear();
    while(b <= (buffer.length() - 34))
    {
        p = buffer.mid(b, 16).trimmed();
        q = buffer.mid(b + 16, 16).trimmed();
        arestas.push_back(Aresta(p, q));
        b += 34;
    }
    return !arestas.isEmpty();
}

bool trHor::parsePontos()
{
    if(!Importa("Ler PONTOS", "C:\\2026\\Soft\\Instâncias\\", "arquivo de pontos (*.pdw)")) return false;
    int b = 0;
    QString id, atr;
    double y, x, z;
    pontos.clear();
    while(b <= (buffer.length() - 70))
    {
        id = buffer.mid(b, 16).trimmed();
        atr = buffer.mid(b + 16, 16).trimmed();
        y = buffer.mid(b + 32, 12).toDouble() / 10000.0;
        x = buffer.mid(b + 44, 12).toDouble() / 10000.0;
        z = buffer.mid(b + 56, 12).toDouble() / 10000.0;
        pontos.push_back(Ponto(id, atr, y, x, z));
        b += 70;
    }
    return !pontos.isEmpty();
}

bool trHor::parseTrhs()
{
    if(!Importa("Ler NOTÁVEIS DE TRAÇADO HORIZONTAL", "C:\\2026\\Soft\\Instâncias\\", "arquivo de traçado hor (*.trc)")) return false;
    pntrhs.clear();
    ns = 0;
    int b = 0;
    QChar c;
    while(b <= (buffer.length() - 42))
    {
        c = *buffer.mid(b + 24, 16).trimmed().data();
        pntrhs.push_back(pnTrh(ns, (buffer.mid(b, 12).toDouble() / 10000.0),(buffer.mid(b + 12, 12).toDouble() / 10000.0), c));
        b += 42;
        ns++;
    }
    return !pntrhs.isEmpty();
}

void trHor::geraFaces()
{
    int n;
    QString v0, v1, v2;
    for(int i = 0; i < arestas.length() - 2; i++)
    {
        n = 0;
        for(int j = i + 1; j < arestas.length() - 1; j++)
        {
            v0 = v1 = v2 = "";
            if(arestas.at(i).vIni == arestas.at(j).vIni)
            {
                v0 = arestas.at(i).vFim;
                v1 = arestas.at(j).vFim;
                v2 = arestas.at(i).vIni;
            }
            else if(arestas.at(i).vIni == arestas.at(j).vFim)
            {
                v0 = arestas.at(i).vFim;
                v1 = arestas.at(j).vIni;
                v2 = arestas.at(i).vIni;
            }
            else if(arestas.at(i).vFim == arestas.at(j).vFim)
            {
                v0 = arestas.at(i).vIni;
                v1 = arestas.at(j).vIni;
                v2 = arestas.at(i).vFim;
            }
            else if(arestas.at(i).vFim == arestas.at(j).vIni)
            {
                v0 = arestas.at(i).vIni;
                v1 = arestas.at(j).vFim;
                v2 = arestas.at(i).vFim;
            }
            if(!v0.isEmpty())
            {
                for(int k = j + 1; k < arestas.length(); k++)
                {
                    if((arestas.at(k).vIni == v0 && arestas.at(k).vFim == v1) ||
                        (arestas.at(k).vIni == v1 && arestas.at(k).vFim == v0))
                    {
                        faces.push_back(Face(v0, v1, v2));
                        n++;
                    }
                }
            }
            if(n > 1) j = arestas.length() - 1;
        }
    }
}

trHor::~trHor()
{
    delete ui;
}
