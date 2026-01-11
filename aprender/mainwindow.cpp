#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_gravando = false;
}

void MainWindow::setLedColor(QString color) {
    ui->ledStatus->setStyleSheet(QString(
                                     "#ledStatus { "
                                     "  background-color: %1; "
                                     "  border: 2px solid #222222; "
                                     "  border-radius: 10px; "
                                     "}"
                                     ).arg(color));
}

void MainWindow::on_pushButton_clicked()
{
    m_gravando = !m_gravando;
    if(m_gravando)
    {

        ui->pushButton->setText(" PARAR E SALVAR");
        ui->pushButton->setStyleSheet(
            "QPushButton { background-color: #d32f2f; color: white; border-radius: 4px; padding: 8px; font-weight: bold; }"
            "QPushButton:hover { background-color: #b71c1c; }"
            );
        // Ícone padrão de "Stop" ou "Close" do Windows/Qt
        ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    }
    else
    {
        ui->pushButton->setText(" INICIAR CAPTURA");
        ui->pushButton->setStyleSheet(
            "QPushButton { background-color: #388E3C; color: white; border-radius: 4px; padding: 8px; font-weight: bold; }"
            "QPushButton:hover { background-color: #2E7D32; }"
            );
        // Ícone padrão de "Play" ou "Seta" do Windows/Qt
        ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    setLedColor("#00FF00"); // Verde limão aceso
    QTimer::singleShot(200, this, [=]() {setLedColor("#444444");});
}

MainWindow::~MainWindow()
{
    delete ui;
}
