#ifndef GLISTA_H
#define GLISTA_H

#include <QString>
#include "gno.h"

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

#endif // GLISTA_H
