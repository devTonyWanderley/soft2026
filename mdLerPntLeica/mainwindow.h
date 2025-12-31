//  Está lendo os pontos sem fazer nenhum tratamento
//  Ancorar aqui pra fazer a linha no modelo padrão e exportar
//  Jogar para o banco, rotinas de uso geral, inclusive parametrizar o importar e exportar
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#define IDIR "C:\\2026\\Soft\\Inst\\"

#include <QMainWindow>
#include "../Banco/geral.h"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString Buffer;

    bool RemoveEsquerda(QString &arg, const QString bnd);
    bool SeparaEntities();

    bool LerProxPonto(QString &id, QString &atr, QString &x, QString &y, QString &z);
};
#endif // MAINWINDOW_H
