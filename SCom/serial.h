#ifndef SERIAL_H
#define SERIAL_H

#define SDIR "C:\\2026\\Soft\\Inst\\"

#include <QMainWindow>
#include <QTimer>
#include <QStyle>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QFileDialog>

/*  Equipamento     Baud Rate	Data Bits	Parity	Stop Bits	Flow Control	Formato Esperado
    Leica TC 407	19200       8       	None	1           XON/XOFF    	GSI-8 ou GSI-16
    Geodetic GD5	9600        8       	None	1           None        	GTS-7 (Topcon)
*/

QT_BEGIN_NAMESPACE
namespace Ui {class Serial;}
QT_END_NAMESPACE

class Serial : public QMainWindow
{
    Q_OBJECT

public:
    Serial(QWidget *parent = nullptr);
    ~Serial();

private slots:
    void onDadosRecebidos();
    void onTimeoutInatividade();

    void on_pushButton_clicked();

private:
    Ui::Serial  *ui;
    QSerialPort *m_serial;
    QTimer      *m_timerTimeout;
    QByteArray   m_bufferBruto;
    bool m_estaGravando;

    void atuPort();
    void atuStatus(const QString msg, bool erro);
    void atuLed(int estado);
    void setInterfaceLock(bool travado);
    void salvarArquivoBruto();
    void configurarPorta();
    void atualizarInterface(bool gravando);

};
#endif // SERIAL_H
