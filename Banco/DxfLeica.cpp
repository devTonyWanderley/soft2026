#include "DxfLeica.h"


bool RemoveEsquerda(QString &arg, const QString bnd)
{
    if(arg.indexOf(bnd) == -1) return false;
    arg = arg.right(arg.length() - arg.indexOf(bnd) - bnd.length());
    return true;
}

bool SeparaEntities(QString &arg)
{
    if(!RemoveEsquerda(arg, "ENTITIES")) return false;
    arg = arg.left(arg.indexOf("ENDSEC"));
    return true;
}

bool LerProxPonto(QString &arg, QString &id, QString &atr, QString &x, QString &y, QString &z)
{
    if(arg.indexOf("CODE") == -1) return false;
    RemoveEsquerda(arg, "CODE");
    RemoveEsquerda(arg, "\n1\n");
    atr = arg.left(arg.indexOf('\n'));
    atr = fillEsquerda(atr, ' ', 8);

    RemoveEsquerda(arg, "PT_ID");
    RemoveEsquerda(arg, "\n1\n");
    id = arg.left(arg.indexOf('\n'));
    id = fillEsquerda(id, ' ', 8);

    RemoveEsquerda(arg, "HEIGHT");
    RemoveEsquerda(arg, "\n1\n");
    z = arg.left(arg.indexOf('\n'));
    z = fillNumero(z, 4, 3);

    RemoveEsquerda(arg, "CO_ORDS");
    RemoveEsquerda(arg, "\n1\n");
    y = arg.left(arg.indexOf('\n'));

    while(*y.data() == ' ') y = y.right(y.length() - 1);
    x = y.left(y.indexOf(' '));
    y = y.right(y.length() - y.indexOf(' '));

    x = fillNumero(x, 6, 3);
    y = fillNumero(y, 7, 3);
    return true;
}
