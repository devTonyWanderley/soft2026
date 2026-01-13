#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QStack>
#include <QTextStream>

/*      --DIAGRAMA DE FLUXO--
graph TD
    A[Início: readyRead] --> B{Bytes disponíveis?}
        B -- Sim --> C[getChar único byte]
        C --> D{É fim de linha? \n \n ou \r\n}
            D -- Não --> E[Acumular no QByteArray]
            E --> B
                D -- Sim --> F[Converter Buffer para String]
            F --> G[Enviar para o Parser DXF]
            G --> H[Limpar Buffer]
            H --> B
                B -- Não --> I[Fim do Slot]
*/

class pnt
{
private:
    QString pNome;
    QString pAtr;
    double pAbc;
    double pOrd;
    double pCota;
public:
    pnt(const QString id = "", const QString atr = "", const double x = 0, const double y = 0, const double z = 0);
    pnt(const pnt &outro);
    void pSetId(const QString id);
    void pSetAtr(const QString atr);
    void pSetX(const double x);
    void pSetY(const double y);
    void pSetZ(const double z);
    QString pGetId();
    QString pGetAtr();
    double pGetX();
    double pGetY();
    double pGetZ();
};

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QByteArray lBuffer;

};

bool Importa
    (
        QByteArray &buf,
        const QString dir = "C:\\Tony\\Soft\\soft2026\\Inst\\",
        const QString filtro = "Arquivo dxf (*.dxf)",
        const QString titulo = "Ler arquivo leica"
        );

#endif // MAINWINDOW_H
