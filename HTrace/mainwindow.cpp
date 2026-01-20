#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parseLPonto();
    parseAresta();
    parseThPonto();
}

bool MainWindow::Importa(QString titulo, QString dir, QString filtro)
{
    QString id = QFileDialog::getOpenFileName(this, titulo, dir, filtro);
    if(id.isEmpty()) return false;
    QFile arq(id);
    if(!arq.open(QIODevice::ReadOnly)) return false;
    Data.clear();
    Data = arq.readAll();
    arq.close();
    return true;
}

void MainWindow::parseLPonto()
{
    if(!(Importa("LPonto", "C:\\Tony\\Soft\\soft2026\\Instâncias\\", "PDW (*.pdw)"))) return;
    int b = 0;
    QString nome, atr;
    double y, x, z;
    pontos.clear();
    while(b <= (Data.length() - 70))
    {
        nome = Data.mid(b, 16).trimmed();
        atr = Data.mid(b + 16, 16).trimmed();
        y = Data.mid(b + 32, 12).toDouble() / 10000.0;
        x = Data.mid(b + 44, 12).toDouble() / 10000.0;
        z = Data.mid(b + 56, 12).toDouble() / 10000.0;
        pontos.push_back(TopoPoint(nome, atr, y, x, z));
        b += 70;
    }
    qDebug()
        << pontos.length()
        << " pontos";
}

void MainWindow::parseAresta()
{
    if(!(Importa("Aresta", "C:\\Tony\\Soft\\soft2026\\Instâncias\\", "ARS (*.ars)"))) return;
    int b = 0;
    QString p, q;
    arestas.clear();
    while(b <= (Data.length() - 34))
    {
        p = Data.mid(b, 16).trimmed();
        q = Data.mid(b + 16, 16).trimmed();
        arestas.push_back(SArs(p, q));
        b += 34;
    }
    qDebug()
        << arestas.length()
        << " arestas";
}

void MainWindow::parseThPonto()
{
    if(!(Importa("trace", "C:\\Tony\\Soft\\soft2026\\Instâncias\\", "TRC (*.trc)"))) return;
    trc.clear();
    int b = 0;
    QChar c;
    while(b <= (Data.length() - 42))
    {
        c = *Data.mid(b + 24, 16).trimmed().data();
        trc.push_back(THPonto((Data.mid(b, 12).toDouble() / 10000.0),(Data.mid(b + 12, 12).toDouble() / 10000.0), c));
        b += 42;
    }
    qDebug()
        << trc.length()
        << " pontos de traçado horizontal";
}

bool MainWindow::getPonto(const QString nome, TopoPoint &res)
{
    for(int i = 0; i < pontos.length(); i++)
    {
        if(pontos.at(i).id == nome)
        {
            res.id = pontos.at(i).id;
            res.attr = pontos.at(i).attr;
            res.n = pontos.at(i).n;
            res.e = pontos.at(i).e;
            res.z = pontos.at(i).z;
            return true;
        }
    }
    return false;
}

bool MainWindow::inteceptaTangente(THPonto * t0, THPonto * t1, SArs * a, TopoPoint & r)
{
    TopoPoint a0, a1;
    if(!getPonto(a->i0,a0)) return false;
    if(!getPonto(a->i1,a1)) return false;
    double u[2], v[2], mu, mv, alfa, beta, dc, da, db;
    u[0] = t1->x - t0->x;
    mu = qPow(u[0], 2.0);
    u[1] = t1->y - t0->y;
    mu += qPow(u[1], 2.0);
    mu = qSqrt(mu);
    u[0] /= mu;
    u[1] /= mu;
    v[0] = a1.e - a0.e;
    mv = qPow(v[0], 2.0);
    v[1] = a1.n - a0.n;
    mv += qPow(v[1], 2.0);
    mv = qSqrt(mv);
    v[0] /= mv;
    v[1] /= mv;
    dc = (u[1] * v[0]) - (u[0] * v[1]);
    if(dc == 0.0) return false;
    da = ((a0.n - t0->y) * v[0]) - ((a0.e - t0->x) * v[1]);
    db = (u[0] * (a0.n - t0->y)) - (v[0] * (a0.e - t0->x));
    alfa = da / dc;
    if((alfa < 0) || (alfa > mu)) return false;
    beta = db / dc;
    if((beta < 0) || (beta > mv)) return false;
    r.id = "calculado";
    r.attr = "pn";
    r.e = a0.e + (beta * v[0]);
    r.n = a0.n + (beta * v[1]);
    r.z = a0.z + ((beta / mv) * (a1.z - a0.z));
    return true;
}

THPonto MainWindow::achaCentro(const THPonto p0, const THPonto p1, const THPonto p2)
{
    double p3[2], m01, m13, m14, e, n, v[2];
    m01 = qSqrt(qPow((p1.x - p0.x), 2.0) + qPow((p1.y - p0.y), 2.0));
    p3[0] = ((p2.x - p0.x) / 2.0) + p0.x;
    p3[1] = ((p2.y - p0.y) / 2.0) + p0.y;
    m13 = qSqrt(qPow((p1.x - p3[0]), 2.0) + qPow((p1.y - p3[1]), 2.0));
    m14 = qPow(m01, 2) / m13;
    v[0] = (p3[0] - p1.x) / m13;
    v[1] = (p3[1] - p1.y) / m13;
    e = p1.x + (v[0] * m14);
    n = p1.y + (v[1] * m14);
    return THPonto(e, n, 'c');
}

bool MainWindow::inteceptaCurva(THPonto * t0, THPonto * t1, THPonto * t2, SArs * a, TopoPoint & r)
{
    TopoPoint a0, a1;
    if(!getPonto(a->i0,a0)) return false;
    if(!getPonto(a->i1,a1)) return false;
    if(a0.e == a1.e)
    {
        qDebug()
            << "O bug da reta vertical";
        return false;
    }
    THPonto tc = achaCentro(*t0, *t1, *t2);
    double aa = ((a1.n - a0.n) / (a1.e - a0.e)), bb = a0.n - (aa * a0.e), rr;
    rr = qSqrt(((qPow((t0->x - tc.x), 2.0)) + (qPow((t0->y - tc.y), 2.0))));
    double alfa = (qPow(aa, 2) + 1.0), beta, gama, delta;
    beta = aa * bb;
    beta -= tc.x;
    beta -= (aa * tc.y);
    beta *= 2.0;
    gama = qPow(tc.x, 2.0);
    gama += qPow(bb, 2.0);
    gama -= (2 * bb * tc.y);
    gama += qPow(tc.y, 2.0);
    gama -= qPow(rr, 2.0);
    delta = qPow(beta, 2.0);
    delta -= (4 * alfa * gama);
    if(delta < 0.0) return false;
    delta = qSqrt(delta);
    double x1 = (delta - beta), x2 = - (delta + beta);
    x1 /= (2 * alfa);
    x2 /= (2 * alfa);
    double y1 = ((aa * x1) + bb), y2 = ((aa * x2) + bb);
    // falta verificar os pontos calculados se pertencem ao trângulo p0 - p1 - p2
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}
