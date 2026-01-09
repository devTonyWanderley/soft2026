#ifndef TIPOS_H
#define TIPOS_H

//  Nos, Lista, Ponto, Face, Contorno, Superfície
//      ->Nos, Lista, Ponto ok...
//      ->  ? vou precisar de aresta? .. simplificar...


#define TIPO_ALFA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TIPO_NUM "0123456789"
#define TIPO_SEP_NUM ",."
#define TIPO_LIGA "-_<>"

#define IDIR "C:\\"

#include <QString>
#include <QFile>
#include <QFileDialog>

//  Manipulação de texto:
QString AddChr(const QString tx, const QChar c, const uint n, bool esq);
QString AddChr(const QString tx, const QString c, const uint n, bool esq);

QString Ignore(const QString tx, const QString exceto);
QString IgnoreEsquerda(const QString tx, const QString exceto);
QString IgnoreDireita(const QString tx, const QString exceto);

QString get_Id_Ln_Pnt(const QString ln);
QString get_Atr_Ln_Pnt(const QString ln);
double get_Abci_Ln_Pnt(const QString ln);
double get_Orde_Ln_Pnt(const QString ln);
double get_Cota_Ln_Pnt(const QString ln);

QString get_Va_Ln_Ars(const QString ln);
QString get_Vb_Ln_Ars(const QString ln);

QString get_Va_Ln_Fce(const QString ln);
QString get_Vb_Ln_Fce(const QString ln);
QString get_Vc_Ln_Fce(const QString ln);

QString SeparaEntities(const QString arg);
bool LerPntDXl(QString &buf, QString &ln);

//  Arquivo
bool Importa(QString &buf, QString dir, QString filtro, QString titulo);
bool Importa(QString &buf, QString filtro, QString titulo);
bool Exporta(QString &buf, QString dir, QString filtro, QString titulo);
bool Exporta(QString &buf, QString filtro, QString titulo);

//  Tipos
template <class G>
class Nos
{
public:
    Nos<G> *Segue;
    G Valor;
    Nos()
    {
        Segue = nullptr;
        Valor = G();
    }

    Nos(const G &arg)
    {
        Segue = nullptr;
        Valor = G(arg);
    }
};

template <class G>
class Lista
{
private:
    Nos<G> *Primo;
    Nos<G> *Fim;
    uint Count;

public:
    Lista()
    {
        Primo = Fim = nullptr;
        Count = 0;
    }

    void Pushfront(const G arg)
    {
        Nos<G> *pont = new Nos<G>(arg);
        if(Primo)
        {
            pont->Segue = Primo;
            Primo = pont;
        }
        else
            Primo = Fim = pont;
        Count++;
    }

    void Pushback(const G arg)
    {
        Nos<G> *pont = new Nos<G>(arg);
        if(Fim)
        {
            Fim->Segue = pont;
            Fim = pont;
        }
        else
            Primo = Fim = pont;
        Count++;
    }

    void Pushn(uint n, const G arg)
    {
        if(!n)
        {
            Pushfront(arg);
            return;
        }
        if(n >= Count)
        {
            Pushback(arg);
            return;
        }
        uint i = 1;
        Nos<G> *pontl = Primo;
        while(i < n)
        {
            pontl = pontl->Segue;
            i++;
        }
        Nos<G> *pont = new Nos<G>(arg);
        pont->Segue = pontl->Segue;
        pontl->Segue = pont;
        Count++;
    }

    Nos<G> * GetPrimo()
    {
        return Primo;
    }

    G Getfront()
    {
        if(Empty())
        {
            G r;
            return r;
        }
        return Primo->Valor;
    }

    G Getback()
    {
        if(Empty())
        {
            G r;
            return r;
        }
        return Fim->Valor;
    }

    G Getn(uint n)
    {
        G r;
        if(Empty()) return r;
        if(!n) return Getfront();
        if(n >= Count) return Getback();
        uint i = 0;
        Nos<G> *pontl = Primo;
        while(i < n)
        {
            pontl = pontl->Segue;
            i++;
        }
        return pontl->Valor;
    }

    G Popfront()
    {
        G r;
        if(Primo)
        {
            Nos<G> *pont = Primo;
            r = pont->Valor;
            if(Primo->Segue)
                Primo = Primo->Segue;
            else
                Primo = Fim = nullptr;
            delete pont;
            Count--;
        }
        return r;
    }

    G Popback()
    {
        G r;
        if(Fim)
        {
            r = Fim->Valor;
            Nos<G> *pont = Primo;
            if(Fim == Primo)
                Primo = Fim = nullptr;
            else
            {
                while(pont->Segue != Fim) pont = pont->Segue;
                Fim = pont;
                pont = pont->Segue;
            }
            delete pont;
            Count--;
        }
        return r;
    }

    G Popn(uint n)
    {
        G r;
        if(Empty()) return r;
        if(!n) return Popfront();
        if(n >= Count) return Popback();
        uint i = 0;
        Nos<G> *pontl = Primo, *ponta;
        while(i < n)
        {
            ponta = pontl;
            pontl = pontl->Segue;
            i++;
        }
        r = pontl->Valor;
        ponta->Segue = pontl->Segue;
        delete pontl;
        Count --;
        return r;
    }

    uint Length()
    {
        return Count;
    }

    bool Empty()
    {
        return (Count == 0);
    }

    void Clear()
    {
        if(Empty()) return;
        Nos<G> *pont;
        while(Primo != Fim)
        {
            pont = Primo;
            Primo = Primo->Segue;
            delete pont;
            Count--;
        }
        pont = Primo;
        Primo = Fim = nullptr;
        delete pont;
        Count--;
    }

    Lista<G> operator =(const Lista<G> outra)
    {
        Clear();
        for(uint i = 0; i < outra.Length(); i++) Pushback(outra.Getn(i));
        return *this;
    }
};

class Ponto
{
private:
    QString Nome;
    QString Atri;
    double Abci;
    double Orde;
    double Cota;
public:
    Ponto();
    Ponto(const QString ln);
    Ponto(const Ponto& outro);
    Ponto(const QString id, const QString atr, const double x, const double y, const double z);
    void SetId(QString id);
    void SetAtr(QString atr);
    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);
    QString GetId();
    QString GetAtr();
    double GetX();
    double GetY();
    double GetZ();
    Ponto operator = (const Ponto &outro);
    bool operator == (const Ponto outro);
};

class SegReta
{
private:
    QString va;
    QString vb;
public:
    SegReta();
    SegReta(const QString a, const QString b);
    SegReta(const QString ln);
    SegReta(const SegReta &outra);
    SegReta operator =(const SegReta &outra);
    bool operator ==(const SegReta outra);
};

class Face
{
private:
    QString va;
    QString vb;
    QString vc;
public:
    Face();
    Face(const QString a, const QString b, const QString c);
    Face(const QString ln);
    Face(const Face &outra);
    Face operator =(const Face &outra);
    bool operator ==(const Face outra);
};

class Superficie
{
private:
    QString nome;
    Lista<Ponto> pontos;
    Lista<Face> faces;
    Lista<QString> contorno;
public:
    Superficie(const QString id = "generica");
    QString getNome();
    void setNome(const QString id);
};

#endif // TIPOS_H
