#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    teste();
}

void MainWindow::teste()
{
    /*
    qDebug()
        << "Teste de QString AddChr(const QString tx, const QChar c, const uint n, bool esq)";
    QString a = AddChr("125", '0', 6, true);
    qDebug()
        << a;
    a = AddChr(a, '.', 7, false);
    qDebug()
        << a;
    a = AddChr(a, '0', 11, false);
    qDebug()
        << a;
    qDebug()
        << "Teste de QString AddChr(const QString tx, const QString, const uint n, bool esq)";
    qDebug()
        << AddChr("a", "ra", 5, false);
    qDebug()
        << AddChr("a", "ar", 5, true);
    QString a = TIPO_ALFA;
    a += TIPO_SEP_NUM;
    qDebug()
        << "Teste de soma de constantes\n"
        << a;
    qDebug()
        << "Teste de QString Ignore(const QString tx, const QString exceto)";
    qDebug()
        << Ignore("abc,012.3-45>67_8def", TIPO_ALFA)
        << '\t'
        << Ignore("abc,012.3-45>67_8def", TIPO_NUM)
        << '\t'
        << Ignore("abc,012.3-45>67_8def", TIPO_SEP_NUM)
        << '\t'
        << Ignore("abc,012.3-45>67_8def", TIPO_LIGA)
        << '\t'
        << "abc,012.3-45>67_8def";
    qDebug()
        << "Teste de QString IgnoreEsquerda(const QString tx, const QString exceto)";
    QString a = TIPO_NUM;
    a = a.right(a.length() - 1);
    a += TIPO_SEP_NUM;
    qDebug()
        << IgnoreEsquerda("000000125,13500", a);
    qDebug()
        << "Teste de QString IgnoreDireita(const QString tx, const QString exceto)";
    qDebug()
        << IgnoreDireita(IgnoreEsquerda("000000125,13500", a), a);
    */
    qDebug()
        << "Teste de QString get_Id_Ln_Pnt(const QString ln)"
        << '\t'
        << get_Id_Ln_Pnt("       0     AUX001000.0000005000.0000050.000");
    qDebug()
        << "Teste de QString get_Atr_Ln_Pnt(const QString ln)"
        << '\t'
        << get_Atr_Ln_Pnt("       0     AUX001000.0000005000.0000050.000");
    qDebug()
        << "Teste de QString get_Abci_Ln_Pnt(const QString ln)"
        << '\t'
        << get_Abci_Ln_Pnt("       0     AUX001000,0000005000.0000050.000");
    qDebug()
        << "Teste de QString get_Orde_Ln_Pnt(const QString ln)"
        << '\t'
        << get_Orde_Ln_Pnt("       0     AUX001000,0000005000,0000050.000");
    qDebug()
        << "Teste de QString get_Cota_Ln_Pnt(const QString ln)"
        << '\t'
        << get_Cota_Ln_Pnt("       0     AUX001000,0000005000,0000050,000");
}

MainWindow::~MainWindow()
{
    delete ui;
}
