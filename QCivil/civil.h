#ifndef CIVIL_H
#define CIVIL_H

//  Implementar a aquisição de pontos com os diversos métodos
//      ->Serial
//      ->Arquivo dxf da leica
//      ->Arquivo pnt

#include <QMainWindow>
#include <QMessageBox>
#include "../Banco/geral.h"
#include "../Banco/forms/serial.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Civil;}
QT_END_NAMESPACE

class Civil : public QMainWindow
{
    Q_OBJECT

public:
    Civil(QWidget *parent = nullptr);
    ~Civil();

private slots:
    void on_actionLeica_triggered();

    void on_actionGD5_triggered();

    void on_actionDxf_leica_triggered();

    void on_actionArs_triggered();

    void on_actionSup_triggered();

    void on_actionTrh_triggered();

    void on_actionPnt_triggered();

private:
    Ui::Civil *ui;
    Serial *cs;
    QString Buffer;
};
#endif // CIVIL_H
