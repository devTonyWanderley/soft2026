#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

struct THPonto
{
    THPonto(const double e, const double n, const QChar det)
    {
        x = e;
        y = n;
        d = det;
    }
    QChar d;
    double x;
    double y;
};

struct TopoPoint {
    TopoPoint()
    {
        id = "";
        attr = "";
        n = 0.0;
        e = 0.0;
        z = 0.0;
    }
    TopoPoint(const QString nm, const QString a, const double y, const double x, const double h)
    {
        id = nm;
        attr = a;
        n = y;
        e = x;
        z = h;
    }
    QString id;
    QString attr;
    double n;
    double e;
    double z;
};

struct SArs
{
    SArs(const QString a, const QString b)
    {
        i0 = a;
        i1 = b;
    }
    QString i0;
    QString i1;
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
    QByteArray Data;
    QList<THPonto> trc;
    QList<TopoPoint> pontos;
    QList<SArs> arestas;

    bool Importa(QString titulo = "Ler arquivo",
                 QString dir = "C:\\Tony\\Soft\\soft2026\\Instâncias\\",
                 QString filtro = "todos (*.*)");
    void parseLPonto();
    void parseAresta();
    void parseThPonto();

    bool getPonto(const QString nome, TopoPoint &res);
    THPonto achaCentro(const THPonto p0, const THPonto p1, const THPonto p2);

    bool inteceptaTangente(THPonto * t0, THPonto * t1, SArs * a, TopoPoint & r);
    bool inteceptaCurva(THPonto * t0, THPonto * t1, THPonto * t2, SArs * a, TopoPoint & r);
};
#endif // MAINWINDOW_H
