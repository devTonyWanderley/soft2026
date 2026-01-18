#ifndef EDITA_H
#define EDITA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class Edita;}
QT_END_NAMESPACE

class Edita : public QMainWindow
{
    Q_OBJECT

public:
    Edita(QWidget *parent = nullptr);
    ~Edita();

private slots:
    void on_pbLer_clicked();

    void on_pbSalvar_clicked();

    void on_pbScr_clicked();

private:
    Ui::Edita *ui;
};
#endif // EDITA_H
