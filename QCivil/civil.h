#ifndef CIVIL_H
#define CIVIL_H

//  Implementar a aquisição de pontos com os diversos métodos
//      ->Serial
//      ->Arquivo dxf da leica
//      ->Arquivo pnt

#include <QMainWindow>
#include <QMessageBox>
//#include "../Banco/geral.h"
#include "../Banco/forms/comserial.h"
#include "../Banco/DxfLeica.h"
#include "../Banco/ed/glista.h"
#include "../Banco/tipos/aresta.h"
#include "../Banco/tipos/face.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Civil;}
QT_END_NAMESPACE

class Civil : public QMainWindow
{
    Q_OBJECT

public:
    QByteArray QbaBuf;          //  a conversão de qbytearray pra qstring pode ser feita no comserial

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
    void on_actionPnt_2_triggered();
    void on_actionSup_2_triggered();
    void on_actionCon_triggered();

private:
    Ui::Civil *ui;
    QString Buffer;
    ComSerial *cs;
    Lista<Ponto> Lp;
    Lista<Aresta> La;

    void GuardaPontos();
    void GuardaArestas();
};
#endif // CIVIL_H
