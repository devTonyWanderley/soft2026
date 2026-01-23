#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::lerPontos_e_Arestas()
{
    QFile arq(ARQP);
    if(!arq.open(QIODevice::ReadOnly)) return false;
    buffer.clear();
    buffer = arq.readAll();
    arq.close();
    qDebug()
        << buffer.length()
        << " bytes de ponto lidos";
    int b = 0;
    QString id, atr;
    double x, y, z;
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
    if(pontos.isEmpty()) return false;
    qDebug()
        << pontos.length()
        << " pontos lidos";
    arq.setFileName(ARQA);
    if(!arq.open(QIODevice::ReadOnly)) return false;
    buffer.clear();
    buffer = arq.readAll();
    arq.close();
    qDebug()
        << buffer.length()
        << " bytes de aresta lidos";
    b = 0;
    QString p, q;
    arestas.clear();
    while(b <= (buffer.length() - 34))
    {
        p = buffer.mid(b, 16).trimmed();
        q = buffer.mid(b + 16, 16).trimmed();
        arestas.push_back(Aresta(p, q));
        b += 34;
    }
    if(arestas.isEmpty()) return false;
    qDebug()
        << arestas.length()
        << " arestas lidas";
}

MainWindow::~MainWindow()
{
    delete ui;
}
