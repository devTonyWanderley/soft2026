#ifndef TRHOR_H
#define TRHOR_H

#include <QMainWindow>
#include <QList>

//  estaquear traçado .. de 5 em 5 na curva e de 10 em 10 na tangente.

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

struct pnTrh
{
    pnTrh(const int s = -1, const double x = 0.0, const double y = 0.0, const QChar tp ='c');
    int serie;
    double X;
    double Y;
    QChar tipo;
};

struct Estaca
{
    Estaca() {}
    double DA = 0.0;
    double este = 0.0;
    double norte = 0.0;
    double cota = 0.0;
    QString obs = "";
};

QT_BEGIN_NAMESPACE
namespace Ui {class trHor;}
QT_END_NAMESPACE

class trHor : public QMainWindow
{
    Q_OBJECT

public:
    trHor(QWidget *parent = nullptr);
    ~trHor();

private:
    Ui::trHor *ui;
    QByteArray buffer;
    QList<Ponto> pontos;
    QList<Aresta> arestas;
    QList<pnTrh> pntrhs;
    QList<Face> faces;
    int ns;

    bool Importa(QString titulo = "Ler arquivo",
                 QString dir = "C:\\2026\\Soft\\Instâncias\\",
                 QString filtro = "todos (*.*)");
    bool parsePontos();
    bool parseArestas();
    bool parseTrhs();

    void geraFaces();
};
#endif // TRHOR_H
