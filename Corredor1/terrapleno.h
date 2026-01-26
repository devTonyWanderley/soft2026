#pragma once

#include <QMainWindow>
#include "Engenharia.h"
#include "StorageProvider.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Terrapleno;}
QT_END_NAMESPACE

class Terrapleno : public QMainWindow
{
    Q_OBJECT

public:
    Terrapleno(QWidget *parent = nullptr);
    ~Terrapleno();

private slots:
    void on_btnProcessar_clicked();

private:
    Ui::Terrapleno *ui;
    Corredor m_corredor; // O motor de cálculo mora aqui
    Superficie m_primitiva; // A malha do terreno
};
