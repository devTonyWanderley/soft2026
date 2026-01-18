#include "edita.h"
#include "ui_edita.h"
#include "../BancoDxf/dxfparse.h"
#include <QFileDialog>
#include <QMessageBox>

Edita::Edita(QWidget *parent): QMainWindow(parent), ui(new Ui::Edita)
{
    ui->setupUi(this);
    ui->ptePontos->setFont(QFont("Consolas", 10));
}

void Edita::on_pbLer_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Importar DXF Leica", "", "DXF (*.dxf)");
    if (path.isEmpty()) return;

    QVector<TopoPoint> pontos = DxfParser::loadAndParse(path);

    if (pontos.isEmpty())
    {
        QMessageBox::warning(this, "Aviso", "Nenhum ponto válido encontrado no DXF.");
        return;
    }

    // Preenche o editor com os 68 caracteres formatados
    ui->ptePontos->clear();
    for (const TopoPoint &p : pontos) {
        ui->ptePontos->appendPlainText(DxfParser::formatToFixedLine(p));
    }
}

void Edita::on_pbSalvar_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Salvar Edição para Desenho", "", "Arquivo PDW (*.pdw)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Latin1);
        out << ui->ptePontos->toPlainText();
        file.close();
    }
}

void Edita::on_pbScr_clicked()
{
    QString conteudo = ui->ptePontos->toPlainText();
    QStringList linhas = conteudo.split('\n', Qt::SkipEmptyParts);

    QVector<TopoPoint> pontosEditados;

    // Converte as linhas de 68 chars de volta para objetos TopoPoint
    for (const QString &linha : linhas)
    {
        if (linha.length() >= 68)
        {
            TopoPoint p;
            p.id = linha.mid(0, 16).trimmed();
            p.attr = linha.mid(16, 16).trimmed();
            p.n = linha.mid(32, 12).toDouble() / 10000.0;
            p.e = linha.mid(44, 12).toDouble() / 10000.0;
            p.z = linha.mid(56, 12).toDouble() / 10000.0;
            pontosEditados.append(p);
        }
    }

    QString scrPath = QFileDialog::getSaveFileName(this, "Salvar Script CAD", "", "Script (*.scr)");
    if (!scrPath.isEmpty()) {
        // Usa o método que já validamos no DxfParser
        DxfParser::fazCadScr(scrPath, pontosEditados);
    }
}

Edita::~Edita()
{
    delete ui;
}
