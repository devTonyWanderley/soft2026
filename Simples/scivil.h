#ifndef SCIVIL_H
#define SCIVIL_H

#include <QMainWindow>
#include "../Banco/forms/comserial.h"
#include "../Banco1/superficie.h"

//	->Importar arquivo dxf da Leica, salvando no formato de 45 caracteres fixos *.pnt:
//      ->Alterar o CMakeLists.txt para acrescentar comunicação serial      ..      ..      ok
//      ->Acrescentar form para comunicação serial          ..      ..      ..      ..      ok
//      ->Fazer leitura de pontos
//          ->Verificar comunicação com a gd5 e mostrar na tela     ..      ..      ..      ok
//          obs.:   a conclusão da tarefa depende da disponibilidade da estação leica
//                  desta forma, cria-se um caminho provisório de salvar um dxf no
//                  formato bruto.
//          ->Salvar arquivo no formato bruto
//	->Importar arquivos *.pnt & *.ars
//	->Gerar faces, contorno, curvas de nível
//	->Exportar faces, contorno, curvas de nível
QT_BEGIN_NAMESPACE
namespace Ui {class SCivil;}
QT_END_NAMESPACE

class SCivil : public QMainWindow
{
    Q_OBJECT

public:
    SCivil(QWidget *parent = nullptr);
    ~SCivil();

private slots:
    void on_pbSerial_clicked();
    void on_pbGera_clicked();
    void on_pbDxf_clicked();
    void on_pbIpnt_clicked();
    void on_pbArs_clicked();
    void on_pbEpnt_clicked();
    void on_pbFce_clicked();
    void on_pbCon_clicked();
    void on_pbCn_clicked();

private:
    Ui::SCivil *ui;
    ComSerial *cs;
    QString Buffer;
    uint Passo;

    Lista<Ponto> Lp;
    Lista<Aresta> La;

    void CarregaPontos(QString buf);
    void CarregaArestas(QString buf);
};
#endif // SCIVIL_H
