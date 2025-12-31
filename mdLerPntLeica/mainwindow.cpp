#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::Importar(QString dir, QString filtro, QString titulo)
{
    QString id = QFileDialog::getOpenFileName(nullptr, titulo, dir, filtro);
    if(id == "") return false;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::ReadOnly | QFile::Text))return false;
    Buffer.clear();
    Buffer = arq.readAll();
    arq.close();
    return true;
}

bool MainWindow::Importar(QString filtro, QString titulo)
{
    return Importar(IDIR, filtro, titulo);
}

bool MainWindow::Exportar(QString dir, QString filtro, QString titulo)
{
    QString id = QFileDialog::getSaveFileName(nullptr, titulo, dir, filtro);
    if(id == "") return false;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::WriteOnly | QFile::Text))return false;
    QTextStream out(&arq);
    out << Buffer;
    arq.close();
    return true;
}

bool MainWindow::Exportar(QString filtro, QString titulo)
{
    return Exportar(IDIR, filtro, titulo);
}

bool MainWindow::RemoveEsquerda(QString &arg, const QString bnd)
{
    if(arg.indexOf(bnd) == -1) return false;
    arg = arg.right(arg.length() - arg.indexOf(bnd) - bnd.length());
    return true;
}

bool MainWindow::SeparaEntities()
{
    if(!RemoveEsquerda(Buffer, "ENTITIES")) return false;
    Buffer = Buffer.left(Buffer.indexOf("ENDSEC"));
    return true;
}

bool MainWindow::LerProxPonto(QString &id, QString &atr, QString &x, QString &y, QString &z)
{
    if(Buffer.indexOf("CODE") == -1) return false;
    RemoveEsquerda(Buffer, "CODE");
    RemoveEsquerda(Buffer, "\n1\n");
    atr = Buffer.left(Buffer.indexOf('\n'));

    RemoveEsquerda(Buffer, "PT_ID");
    RemoveEsquerda(Buffer, "\n1\n");
    id = Buffer.left(Buffer.indexOf('\n'));

    RemoveEsquerda(Buffer, "HEIGHT");
    RemoveEsquerda(Buffer, "\n1\n");
    z = Buffer.left(Buffer.indexOf('\n'));

    RemoveEsquerda(Buffer, "CO_ORDS");
    RemoveEsquerda(Buffer, "\n1\n");
    y = Buffer.left(Buffer.indexOf('\n'));

    while(*y.data() == ' ') y = y.right(y.length() - 1);
    x = y.left(y.indexOf(' '));
    y = y.right(y.length() - y.indexOf(' '));

    while(*y.data() == ' ') y = y.right(y.length() - 1);
    return true;
}

void MainWindow::on_pushButton_clicked()
{
    if(!Importar("dxf (*.dxf)", "Ler pontos do dxf")) return;
    if(!SeparaEntities()) return;
    QString id, atr, x, y, z;
    while(LerProxPonto(id, atr, x, y, z))
        qDebug()
            << id
            << '\t'
            << atr
            << '\t'
            << x
            << '\t'
            << y
            << '\t'
            << z;
}

MainWindow::~MainWindow()
{
    delete ui;
}
