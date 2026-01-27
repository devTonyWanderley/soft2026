#include "terrapleno.h"
#include "ui_terrapleno.h"

Terrapleno::Terrapleno(QWidget *parent): QMainWindow(parent), ui(new Ui::Terrapleno)
{
    ui->setupUi(this);
}

void Terrapleno::on_btnProcessar_clicked() {
    // 1. Definição de Caminhos
    QString dir = "C:/2026/NovaSede/";
    ui->statusbar->showMessage("🚀 Iniciando Processamento Modular...");

    // 2. Configuração de Layouts
    FileLayout pPdw;
    pPdw.escala = 10000.0;
    pPdw.colunas["ID"]={0,16}; pPdw.colunas["Y"]={32,12}; pPdw.colunas["X"]={44,12}; pPdw.colunas["Z"]={56,12};

    // 3. CARGA DE DADOS (A Rocha)
    m_primitiva.pontos = StorageProvider::lerPontos(dir + "Pontos.pdw", pPdw);
    StorageProvider::lerArestas(dir + "Arestas.ars", m_primitiva);

    // 4. CONSTRUÇÃO TOPOLÓGICA (A Ordem Crítica)
    // Primeiro as Faces, depois o Contorno que depende delas
    ui->statusbar->showMessage("🧩 Reconstruindo Malha e Contorno...");
    m_primitiva.reconstruirFaces();      // Sua lógica O(n²)
    m_primitiva.gerarContornoSequencial(); // Lógica Cabeça e Cauda

    // 5. AUDITORIA FINAL NO CONSOLE
    qDebug() << "-------------------------------------------";
    qDebug() << "RESULTADO DA RECONSTRUÇÃO:";
    qDebug() << "   Pontos:   " << m_primitiva.pontos.size();
    qDebug() << "   Arestas:  " << m_primitiva.arestas.size();
    qDebug() << "   Faces:    " << m_primitiva.faces.size();
    qDebug() << "   Contorno: " << m_primitiva.iContorno.size();
    //qDebug() << "   Contorno: " << m_primitiva.indicesContorno.size() << " vértices encadeados.";
    qDebug() << "-------------------------------------------";

    if (m_primitiva.indicesContorno.empty()) {
        ui->statusbar->showMessage("⚠️ ALERTA: Contorno não fechou. Verifique a malha.");
    } else {
        ui->statusbar->showMessage("✅ Sucesso! Malha e Contorno íntegros.");
    }
    //for(int i = 0; i < m_primitiva.arestas.size(); i++)
    //    qDebug() << m_primitiva.arestas.at(i).iIni << " -> " << m_primitiva.arestas.at(i).iFim;
}

Terrapleno::~Terrapleno()
{
    delete ui;
}
