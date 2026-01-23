#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ARQP "C:\\Tony\\Soft\\soft2026\\Instâncias\\Pontos.pdw"
#define ARQA "C:\\Tony\\Soft\\soft2026\\Instâncias\\Arestas.ars"

#include <QMainWindow>
#include <QList>

//  Ler pontos e arestas

struct Ponto
{
    Ponto(const QString id = "", const QString atr = "", const double x = 0.0, const double y = 0.0, const double z = 0.0);
    QString nome;
    QString atri;
    double abci;
    double orde;
    double cota;
};

struct Aresta
{
    Aresta(const QString v0 = "", const QString v1 = "");
    QString vIni;
    QString vFim;
};

struct Face
{
    Face(const QString aa = "", const QString bb = "", const QString cc = "");
    QString va;
    QString vb;
    QString vc;
};

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QByteArray buffer;
    QList<Ponto> pontos;
    QList<Aresta> arestas;

    bool lerPontos_e_Arestas();
};
#endif // MAINWINDOW_H
