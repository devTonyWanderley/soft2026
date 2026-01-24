#ifndef TESTADOR_H
#define TESTADOR_H

#include <QMainWindow>

/*
   TODO: ROADMAP DE IMPORTAÇÃO
   - [ ] Implementar extrairDelimitado (CSV/TXT) no ImportEngine
   - [ ] Criar classe ImportadorDXF : public IProvedorPontos
   - [ ] Criar Parser de Script (JSON/INI) para automação da fila (executarBatch)
   - [ ] Implementar ImportadorMalhaCAD: Capaz de ler entidades 3DFACE do DXF e mapear os vértices para o nosso repositório.

*/

QT_BEGIN_NAMESPACE
namespace Ui {class testador;}
QT_END_NAMESPACE

class testador : public QMainWindow
{
    Q_OBJECT

public:
    testador(QWidget *parent = nullptr);
    ~testador();

private:
    Ui::testador *ui;
};
#endif // TESTADOR_H
