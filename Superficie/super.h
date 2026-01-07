#ifndef SUPER_H
#define SUPER_H

//  Ler pontos
//  Ler arestas
//  Criar faces
//  Criar contorno
//  Exportar superfície
//  Exportar contorno

#include <QMainWindow>
#include "../Banco1/superficie.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Super;}
QT_END_NAMESPACE

class Super : public QMainWindow
{
    Q_OBJECT

public:
    Super(QWidget *parent = nullptr);
    ~Super();

private slots:
    void on_pbOp_clicked();

private:
    Ui::Super *ui;
    QString Buffer;
    Superficie sup;
    uint passo;
};
#endif // SUPER_H
