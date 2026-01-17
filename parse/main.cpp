#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QTranslator>
#include <QElapsedTimer>
#include "../BancoDxf/dxfparse.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QCoreApplication a(argc, argv);

    // 1. Defina o caminho do seu arquivo de teste
    QString arquivoDxf = "C:/2026/Soft/Instâncias/LEV_20260116_142028.dxf";
    QString arquivoSaida = "C:/2026/Soft/Instâncias/resultado.pnt";

    qDebug() << "--- Iniciando Validacao parse ---";

    QElapsedTimer timer;
    timer.start();

    // 2. Executa a carga e o parsing
    QVector<TopoPoint> pontos = DxfParser::loadAndParse(arquivoDxf);

    if (pontos.isEmpty())
    {
        qDebug() << "ERRO: Nenhum ponto encontrado ou arquivo inacessivel.";
        return -1;
    }

    // 3. Valida a contagem (Esperado: 532)
    qDebug() << "Pontos identificados:" << pontos.size();
    qDebug() << "Tempo de processamento:" << timer.elapsed() << "ms";

    // 4. Salva o arquivo de comprimentos fixos
    if (DxfParser::saveToPnt(arquivoSaida, pontos)) {
        qDebug() << "Arquivo .pnt gerado com sucesso em:" << arquivoSaida;
    } else {
        qDebug() << "ERRO ao gravar o arquivo de saída.";
    }

    // 5. Exibe os dois primeiros pontos para conferência visual da máscara
    for (int i = 0; i < qMin(pontos.size(), 2); ++i) {
        qDebug() << "Amostra" << i+1 << ":" << DxfParser::formatToFixedLine(pontos[i]);
    }

    qDebug() << "--- Teste Concluido ---";

    // Como é um teste de console, podemos encerrar após o log
    return 0;

/*
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "parse_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();*/
}
