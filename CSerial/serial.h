#ifndef SERIAL_H
#define SERIAL_H

#define SDIR "C:\\2026\\Soft\\Inst\\"

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

//-------------------------------------------------------------------------------------------------
//  Estados e eventos:
//      nota:   inicia *Desconectado
//      *Desconectado       |refresh, alterar combos |
//          ?porta válida?
//              sim!    ->  *Desconectado
//              não!    ->  *Erro
//      *Desconectado       |pbMain |
//          ?porta válida?
//              sim!    ->  *Aguardando
//              não!    ->  *Erro
//      *Erro               |refresh, alterar combos |
//          ?porta válida?
//              sim!    ->  *Desconectado
//              não!    ->  *Erro
//      *Aguardando         |onDdsRec();    |
//              sim!    ->  *Recebendo
//              não!    ->  *Aguardando
//      *Recebendo          |onDdsRec();    |
//              sim!    ->  *Recebendo
//              não!    ->  *Desconectado   ..  se há dados, salva arquivo.
//      *Recebendo          |pbMain |       ..  interrompe recepção & se há dados, salva arquivo.
//              ..      ->  *Desconectado
//-------------------------------------------------------------------------------------------------
//  Estados e componentes gráficos:
//      cbPort:
//          *Desconectado               ..  habilitado, aparência normal
//          *Aguardando & *Recebendo    ..  desabilitado, aparência normal
//          *Erro                       ..  desabilitado, "Porta inválida", aparência de alerta
//      demais combos:
//          *Desconectado               ..  habilitados
//          *demais estados             ..  desabilitados
//      pteMoni:                        ..  não altera
//      lbStatus:
//          *Desconectado               ..  "Aguarda conexão", aparência normal
//          *Aguardando                 ..  "Aguarda dados", aparência normal
//          *Recebendo                  ..  "% bytes recebidos", aparência normal
//          *Erro                       ..  "ERRO: porta inválida", aparência de alerta
//      pbMain:
//          *Desconectado               ..  habilitado, "Conectar", aparência normal, ícone
//          *Aguardando                 ..  habilitado, "Desconectar", aparência normal, ícone
//          *Recebendo                  ..  habilitado, "Parar / salvar", aparência azul, ícone
//          *Erro                       ..  desabilitado, "...", aparência de alerta
//      pbRefresh:
//          ...                         ..  habilitado
//          *Recebendo                  ..  desabilitado
//-------------------------------------------------------------------------------------------------

enum class mEstado
{
    Desconectado,
    Aguardando,
    Recebendo,
    Erro
};
// E no private: adicione a variável mEstadoAtual;


class Serial : public QMainWindow
{
    Q_OBJECT

public:
    Serial(QWidget *parent = nullptr);
    ~Serial();

private slots:
    void onDdsRec();
    void onInativo();
    void on_tbRefresh_clicked();
    void on_pbMain_clicked();

private:
    Ui::Serial *ui;
    QSerialPort *mSerial;
    QTimer *mTimerOut;
    QByteArray mBuffer;
    bool mGravando;
    mEstado mEstadoAtual;
    void atuCbFixos();

    void mSetEstado(mEstado novoEstado);
    void atuPorts();
    void atuStts(const QString msg, bool erro);
    void atuInter(bool gravando);
    void configPorta();
    void salva();
};
#endif // SERIAL_H
