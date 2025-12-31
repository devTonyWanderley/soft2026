#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    atr = fillEsquerda(atr, ' ', 8);

    RemoveEsquerda(Buffer, "PT_ID");
    RemoveEsquerda(Buffer, "\n1\n");
    id = Buffer.left(Buffer.indexOf('\n'));
    id = fillEsquerda(id, ' ', 8);

    RemoveEsquerda(Buffer, "HEIGHT");
    RemoveEsquerda(Buffer, "\n1\n");
    z = Buffer.left(Buffer.indexOf('\n'));
    z = fillNumero(z, 4, 3);

    RemoveEsquerda(Buffer, "CO_ORDS");
    RemoveEsquerda(Buffer, "\n1\n");
    y = Buffer.left(Buffer.indexOf('\n'));

    while(*y.data() == ' ') y = y.right(y.length() - 1);
    x = y.left(y.indexOf(' '));
    y = y.right(y.length() - y.indexOf(' '));

    x = fillNumero(x, 6, 3);
    y = fillNumero(y, 7, 3);
    return true;
}

void MainWindow::on_pushButton_clicked()
{
    if(!Importa(Buffer, "dxf (*.dxf)", "Ler pontos do dxf")) return;
    if(!SeparaEntities()) return;
    QString id, atr, x, y, z, buf = "";
    while(LerProxPonto(id, atr, x, y, z))
    {
        if(buf.length()) buf += '\n';
        buf += (id + atr + x + y + z);
    }
    Exporta(buf,"pnt (*.pnt)", "Salvar pontos pnt");
}

MainWindow::~MainWindow()
{
    delete ui;
}
