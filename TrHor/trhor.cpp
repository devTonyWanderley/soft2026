#include "trhor.h"
#include "ui_trhor.h"
#include <QFile>
#include <QFileDialog>
#include <QPointF>
#include <QLineF>
#include <QtMath>

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
    if(!parseArestas1())
    {
        qDebug()
            << "Problemas com arestas";
        return;
    }
    qDebug()
        << arestas.length()
        << " arestas lidas";
    if(!parsePontos1())
    {
        qDebug()
        << "Problemas com pontos";
        return;
    }
    qDebug()
        << pontos.length()
        << " pontos lidos";
    /*
    if(!parseTrhs())
    {
        qDebug()
        << "Problemas com pontos";
        return;
    }
    qDebug()
        << pntrhs.length()
        << " pontos notáveis lidos";

    */
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

bool trHor::parseArestas1()
{
    //  C:\\Tony\\Soft\\soft2026\\Instâncias\\Arestas.ars
    QFile arq("C:\\Tony\\Soft\\soft2026\\Instâncias\\Arestas.ars");
    if(!arq.open(QIODevice::ReadOnly)) return false;
    buffer.clear();
    buffer = arq.readAll();
    arq.close();
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

bool trHor::parsePontos1()
{
    //  C:\\Tony\\Soft\\soft2026\\Instâncias\\Pontos.pdw
    QFile arq("C:\\Tony\\Soft\\soft2026\\Instâncias\\Pontos.pdw");
    if(!arq.open(QIODevice::ReadOnly)) return false;
    buffer.clear();
    buffer = arq.readAll();
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
        pontos.push_back(Ponto(id, atr, x, y, z));
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

Ponto trHor::getPonto(const QString id)
{
    for(int i = 0; i < pontos.length(); i++)
        if(pontos.at(i).nome == id) return pontos.at(i);
    return Ponto();
}

double trHor::det3x3(double A[3][3])
{
    double r = (A[0][0] * A[1][1] * A[2][2]);
    r += (A[1][0] * A[2][1] * A[0][2]);
    r += (A[2][0] * A[0][1] * A[1][2]);
    r -= (A[0][2] * A[1][1] * A[2][0]);
    r -= (A[1][0] * A[0][1] * A[2][2]);
    r -= (A[1][2] * A[2][1] * A[0][0]);
    return r;
}

void trHor::coefOmega(const Face f, const double xp, const double yp, double &w0, double &w1, double &w2)
{
    Ponto ta = getPonto(f.va), tb = getPonto(f.vb), tc = getPonto(f.vc);
    double m[3][3], dCar, dw0, dw1, dw2;
    m[0][0] = m[0][1] = m[0][2] = 1.0;
    m[1][0] = ta.abci;
    m[1][1] = tb.abci;
    m[1][2] = tc.abci;
    m[2][0] = ta.orde;
    m[2][1] = tb.orde;
    m[2][2] = tc.orde;
    dCar = det3x3(m);
    m[1][0] = xp;
    m[2][0] = yp;
    dw0 = det3x3(m);
    m[1][0] = ta.abci;
    m[2][0] = ta.orde;
    m[1][1] = xp;
    m[2][1] = yp;
    dw1 = det3x3(m);
    m[1][1] = tb.abci;
    m[2][1] = tb.orde;
    m[1][2] = xp;
    m[2][2] = yp;
    dw2 = det3x3(m);
    w0 = dw0 / dCar;
    w1 = dw1 / dCar;
    w2 = dw2 / dCar;
    /*
    w0 = ((tb.orde - ta.orde) * (tc.abci - ta.abci));
    w0 += ((tc.orde - ta.orde) * (tb.abci - ta.abci));
    w2 = ((tb.orde - ta.orde) * (xp - ta.abci));
    w2 /= w0;
    w1 = (xp - ta.abci);
    w1 -= (w2 * (tc.abci - ta.abci));
    w1 /= (tb.abci - ta.abci);
    w0 = 1.0;
    w0 -= w1;
    w0 -= w2;
    */
    qDebug()
        << w0
        << '\t'
        << w1
        << '\t'
        << w2;
}

bool trHor::PontoInFace_(const Face f, const double x, const double y)
{
    double c0, c1, c2;
    coefOmega(f, x, y, c0, c1, c2);
    if((c0 < 0.0) || (c0 > 1.0)) return false;
    if((c1 < 0.0) || (c1 > 1.0)) return false;
    if((c2 < 0.0) || (c2 > 1.0)) return false;
    return true;
}

Face trHor::getFaceDeP(const double x, const double y)
{
    for(int i = 0; i < faces.length(); i++)
    {
        if(PontoInFace_(faces.at(i), x, y))
            return faces.at(i);
    }
    return Face();
}

void trHor::coefPlano(const Face f, double &a, double &b, double &c, double &d)
{
    Ponto pa = getPonto(f.va), pb = getPonto(f.vb), pc = getPonto(f.vc);
    double u[3], v[3], uv[3], muv;
    u[0] = pb.abci - pa.abci;
    u[1] = pb.orde - pa.orde;
    u[2] = pb.cota - pa.cota;
    v[0] = pc.abci - pa.abci;
    v[1] = pc.orde - pa.orde;
    v[2] = pc.cota - pa.cota;
    uv[0] = ((u[1] * v[2]) - (u[2] * v[1]));
    uv[1] = ((u[2] * v[0]) - (u[0] * v[2]));
    uv[2] = ((u[0] * v[1]) - (u[1] * v[0]));
    muv = qPow(uv[0], 2.0);
    muv += qPow(uv[1], 2.0);
    muv += qPow(uv[2], 2.0);
    muv = qSqrt(muv);
    a = uv[0] / muv;
    b = uv[1] / muv;
    c = uv[2] / muv;
    d = 0.0;
    d -= (a * pa.abci);
    d -= (b * pa.orde);
    d -= (c * pa.cota);
}

double trHor::cotaDoPonto(const Face f, const double x, const double y)
{
    double a, b, c, d, z;
    coefPlano(f, a, b, c, d);
    z = 0.0;
    z -= d;
    z -= (a * x);
    z -= (b * y);
    z /= c;
    return z;
}

void trHor::on_pbOpera_clicked()
{
    double x = ui->leAbc->text().toDouble(), y = ui->leOrd->text().toDouble(), z;
    /*
    qDebug()
        << x
        << '\t'
        << y;
    */
    Face f = getFaceDeP(x, y);
    qDebug()
        << f.va
        << '\t'
        << f.vb
        << '\t'
        << f.vc;
    if(f.va.isEmpty())
    {
        ui->lbCota->setText("Ponto fora da área");
        return;
    }
    z = cotaDoPonto(f, x, y);
    ui->lbCota->setText(QString::number(z));
}

trHor::~trHor()
{
    delete ui;
}
