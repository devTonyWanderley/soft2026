#include "geral.h"

//  Manipulação de texto
QString AddChr(const QString tx, const QChar c, const uint n, bool esq)
{
    if(tx.length() >= n)
    {
        if(esq)
            return tx.right(n);
        else
            return tx.left(n);
    }
    QString r = tx;
    while(r.length() < n)
        if(esq)
            r = c + r;
        else
            r += c;
    if(r.length() > n)
    {
        if(esq)
            return r.right(n);
        else
            return r.left(n);
    }
    return r;
}

QString AddChr(const QString tx, const QString c, const uint n, bool esq)
{
    if(tx.length() >= n)
    {
        if(esq)
            return tx.right(n);
        else
            return tx.left(n);
    }
    QString r = tx;
    while(r.length() < n)
        if(esq)
            r = c + r;
        else
            r += c;
    if(r.length() > n)
    {
        if(esq)
            return r.right(n);
        else
            return r.left(n);
    }
    return r;
}

QString Ignore(const QString tx, const QString exceto)
{
    QString r = "";
    for(uint i = 0; i < tx.length(); i++)
        if(exceto.indexOf(tx.at(i)) != -1)
            r += tx.at(i);
    return r;
}

QString IgnoreEsquerda(const QString tx, const QString exceto)
{
    QString r = tx;
    while((r.length()) && (exceto.indexOf(r.at(0))) == -1)
        r = r.right(r.length() - 1);
    return r;
}

QString IgnoreDireita(const QString tx, const QString exceto)
{
    QString r = tx;
    while((r.length()) && (exceto.indexOf(r.at(r.length() - 1))) == -1)
        r = r.left(r.length() - 1);
    return r;
}

QString get_Id_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.left(8), ig);
}

QString get_Atr_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.right(ln.length() - 8).left(8), ig);
}

double get_Abci_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(ln.length() - 16).left(10);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}

double get_Orde_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(ln.length() - 26).left(11);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}

double get_Cota_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(8);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}

QString get_Va_Ln_Ars(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.left(8), ig);
}

QString get_Vb_Ln_Ars(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.right(8), ig);
}

QString get_Va_Ln_Fce(const QString ln)
{
    return get_Va_Ln_Ars(ln);
}

QString get_Vb_Ln_Fce(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.right(16).left(8), ig);
}

QString get_Vc_Ln_Fce(const QString ln)
{
    return get_Vb_Ln_Ars(ln);
}

QString SeparaEntities(const QString arg)
{
    QString r = arg.right(arg.length() - arg.indexOf("ENTITIES"));
    r = r.left(r.indexOf("ENDSEC"));
    return r;
}

bool LerPntDXl(QString &buf, QString &ln)
{
    if(buf.indexOf("ENTITIES"))
        buf = SeparaEntities(buf);
    if(buf.indexOf("CODE") == -1)
        return false;
    ln.clear();
    buf = buf.right(buf.length() - buf.indexOf("CODE"));
    buf = buf.right(buf.length() - buf.indexOf("\n1\n") - 3);
    QString atr = buf.left(buf.indexOf('\n'));

    buf = buf.right(buf.length() - buf.indexOf("PT_ID"));
    buf = buf.right(buf.length() - buf.indexOf("\n1\n") - 3);
    QString id = buf.left(buf.indexOf('\n'));

    buf = buf.right(buf.length() - buf.indexOf("HEIGHT"));
    buf = buf.right(buf.length() - buf.indexOf("\n1\n") - 3);
    QString h = buf.left(buf.indexOf('\n'));

    buf = buf.right(buf.length() - buf.indexOf("CO_ORDS"));
    buf = buf.right(buf.length() - buf.indexOf("\n1\n") - 3);
    QString y = buf.left(buf.indexOf('\n'));

    QString ig = TIPO_NUM;
    ig += TIPO_SEP_NUM;
    y = IgnoreEsquerda(y, ig);

    QString x = y.left(y.indexOf(' '));
    y = y.right(y.length() - y.indexOf(' '));
    y = IgnoreEsquerda(y, ig);

    x = Ignore(x, ig);
    y = Ignore(y, ig);
    h = Ignore(h, ig);

    ig = TIPO_ALFA;
    ig += TIPO_NUM;
    //ig += TIPO_LIGA;
    Ignore(id, ig);
    Ignore(atr, ig);

    while((x.length() - x.indexOf('.')) > 4) x = x.left(x.length() - 1);
    while((y.length() - y.indexOf('.')) > 4) y = y.left(y.length() - 1);
    while((h.length() - h.indexOf('.')) > 4) h = h.left(h.length() - 1);

    id = AddChr(id, ' ', 8, true);
    atr = AddChr(atr, ' ', 8, true);
    x = AddChr(x, '0', 10, true);
    y = AddChr(y, '0', 11, true);
    h = AddChr(h, '0', 8, true);

    ln = id + atr + x + y + h;
    return true;
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

