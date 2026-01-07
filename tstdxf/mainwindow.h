#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include "../Banco1/dxf.h"

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

    bool Importa(QString &buf, QString dir, QString filtro, QString titulo);
    bool Importa(QString &buf, QString filtro, QString titulo);

    void leitura(QString &buf);
};
#endif // MAINWINDOW_H
