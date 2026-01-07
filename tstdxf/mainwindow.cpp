#include "mainwindow.h"
#include "ui_mainwindow.h"

#define IDIR "C:\\2026\\Soft\\Inst\\"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::Importa(QString &buf, QString dir, QString filtro, QString titulo)
{
    QString id = QFileDialog::getOpenFileName(nullptr, titulo, dir, filtro);
    if(id == "") return false;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::ReadOnly | QFile::Text))return false;
    QByteArray qba = arq.readAll();
    //buf.clear();
    buf = QString::fromUtf8(qba);
    //buf = arq.readAll();
    arq.close();
    return true;
}

bool MainWindow::Importa(QString &buf, QString filtro, QString titulo)
{
    return Importa(buf, IDIR, filtro, titulo);
}

void MainWindow::leitura(QString &buf)
{
    // mostrar linhas separadas
    QString tx[4] = {"", "", "", ""};
    bool ent = false;
    while (buf.indexOf('\n') != -1)
    {
        if(buf.left(1) == "\n") buf = buf.right(buf.length() - 1);
        tx[3] = tx[2];
        tx[2] = tx[1];
        tx[1] = tx[0];
        tx[0] = buf.left(buf.indexOf('\n'));
        while(tx[0].left(1) == " ") tx[0] = tx[0].right(tx[0].length() - 1);
        while(tx[0].right(1) == " ") tx[0] = tx[0].left(tx[0].length() - 1);
        buf = buf.right(buf.length() - buf.indexOf('\n'));
        if(tx[3] == "0")
        {
            if(ent)
            {
                qDebug() << tx[3] << '\t' << tx[2] << '\t' << tx[1] << '\t' << tx[0];
            }
            if(tx[2] == "SECTION")
            {
                qDebug() << tx[3] << '\t' << tx[2] << '\t' << tx[1] << '\t' << tx[0];
                if(tx[1] == "2" && tx[0] == "ENTITIES") ent = true;
            }
            else if(tx[2] == "ENDSEC")
            {
                qDebug() << tx[3] << '\t' << tx[2];
                ent = false;
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString buf;
    if(!Importa(buf, "(*.dxf)", "----")) return;
    //leitura(buf);
    pares(buf);
    /*
    qDebug()
        << "original:\t"
        << buf.length();
    buf = buf.right(buf.length() - buf.indexOf("ENTITIES") - 8);
    qDebug()
        << "Depois de entities:\t"
        << buf.length();
    buf = buf.left(buf.indexOf("ENDSEC"));
    qDebug()
        << "antes de endsec:\t"
        << buf.length();
    qDebug()
        << buf;
    uint cod = buf.left(buf.indexOf('\n')).toUInt();
    buf = buf.right(buf.length() - buf.indexOf('\n') - 1);
    QString val = buf.left(buf.indexOf('\n'));
    buf = buf.right(buf.length() - buf.indexOf('\n') - 1);
    while(val != "ENDSEC")
    {
        if(!cod)
        {
            qDebug()
                << cod
                << '\t'
                << val;
        }
        else
        {
            qDebug()
                << '\t'
                << cod
                << '\t'
                << val;
        }
        cod = buf.left(buf.indexOf('\n')).toUInt();
        buf = buf.right(buf.length() - buf.indexOf('\n') - 1);
        val = buf.left(buf.indexOf('\n'));
        buf = buf.right(buf.length() - buf.indexOf('\n') - 1);
    }
    qDebug()
        << buf.left(20)
        << '\t'
        << cod
        << '\t'
        << val;
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
