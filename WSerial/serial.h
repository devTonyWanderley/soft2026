#ifndef SERIAL_H
#define SERIAL_H

#define SDIR "C:\\Tony\\Soft\\soft2026\\Inst\\"

#include <QMainWindow>
#include <QTimer>
#include <QStyle>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {class Serial;}
QT_END_NAMESPACE

enum class mEstado
{
    Desconectado,
    Aguardando,
    Recebendo,
    Erro
};

class Serial : public QMainWindow
{
    Q_OBJECT

public:
    Serial(QWidget *parent = nullptr);
    ~Serial();

private slots:
    void on_pbMain_clicked();
    void on_tbRefresh_clicked();
    void on_cbPort_currentIndexChanged(int index);
    void on_cbBaud_currentIndexChanged(int index);
    void on_cbDtBit_currentIndexChanged(int index);
    void on_cbPar_currentIndexChanged(int index);
    void on_cbStop_currentIndexChanged(int index);
    void on_cbFlow_currentIndexChanged(int index);

    void onSDados();    //  Recebe os dados
    void onInativo();

private:
    Ui::Serial *ui;
    QSerialPort *mSerial;
    QTimer *mTimer;
    mEstado mEAtu;
    QByteArray mBuffer;

    bool mIniciado;

    void mSetEstado(mEstado estado);    //  Gerencia fluxo
    void mIniDlg();                     //  Inicia os comboboxes e atribui o ícone de tbRefresh
    bool mAtuPorts();                   //  Atualiza cbPort
    void mSetCombos(bool f = true);     //  Habilita / desabilita comboboxes, conforme 'f'
    void mCfgPorta();                   //  Configura porta serial com os valore escolhidos
    void mExporta();                    //  Salva o arquivo bruto (*.bru)
};
#endif // SERIAL_H
