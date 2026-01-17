#ifndef CAMPO_H
#define CAMPO_H

#define SDIR "C:\\2026\\Soft\\Instâncias\\"

#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>
#include "../Banco/cfgcom.h"

//  Ajustar a aparência ..  iniciado    ..  continuar refinando.
//  Ajustar estados e eventos de configuração de porta

QT_BEGIN_NAMESPACE
namespace Ui {class Campo;}
QT_END_NAMESPACE

class Campo : public QMainWindow
{
    Q_OBJECT

public:
    Campo(QWidget *parent = nullptr);
    ~Campo();

private slots:
    void on_cbEdita_checkStateChanged(const Qt::CheckState &arg1);
    void on_pbCfg_clicked();

    void onSDados();    //  Recebe os dados
    void onInativo();

private:
    Ui::Campo *ui;
    cfgCom *cfgS;
    QSerialPort *pontSerial;
    bool *pontFlg;
    QTimer *timer;
    QByteArray buffer;

    void redim(int b = 415, int h = 145);
    void exporta();
    void opera();
};
#endif // CAMPO_H
