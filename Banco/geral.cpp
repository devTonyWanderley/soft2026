#include "geral.h"

QString fillEsquerda(QString tx, QChar c, uint n)
{
    if(tx.length() >= n) return tx.right(n);
    while(tx.length() < n) tx = c + tx;
    return tx;
}

QString fillDireita(QString tx, QChar c, uint n)
{
    if(tx.length() >= n) return tx.left(n);
    while(tx.length() < n) tx += c;
    return tx;
}

QString fillNumero(QString tx, uint ni, uint nf)    //  pode ser melhorado
{
    QString pi = tx, pf;
    if(pi.indexOf('.') != -1)
    {
        pf = pi.right(pi.length() - pi.indexOf('.') - 1);
        pi = pi.left(pi.indexOf('.'));
    }
    else if(pi.indexOf(',') != -1)
    {
        pf = pi.right(pi.length() - pi.indexOf(',') - 1);
        pi = pi.left(pi.indexOf(','));
    }
    else
    {
        pi = tx;
        pf = "";
    }
    QChar *c = pi.data();
    tx = "";
    while(*c != 0)
    {
        if(*c >= '0' && *c <= '9') tx += *c;
        c++;
    }
    return (fillEsquerda(tx, '0', ni) + '.' + fillDireita(pf, '0', nf));
}


bool Importa(QString &buf, QString dir, QString filtro, QString titulo)
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

bool Importa(QString &buf, QString filtro, QString titulo)
{
    return Importa(buf, IDIR, filtro, titulo);
}

bool Exporta(QString &buf, QString dir, QString filtro, QString titulo)
{
    QString id = QFileDialog::getSaveFileName(nullptr, titulo, dir, filtro);
    if(id == "") return false;
    QFile arq;
    arq.setFileName(id);
    if(!arq.open(QFile::WriteOnly | QFile::Text))return false;
    QTextStream out(&arq);
    out << buf;
    arq.close();
    return true;
}

bool Exporta(QString &buf, QString filtro, QString titulo)
{
    return Exporta(buf, IDIR, filtro, titulo);
}
