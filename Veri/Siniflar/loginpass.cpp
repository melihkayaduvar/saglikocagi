#include "loginpass.h"

loginpass::loginpass():IdliSinif(),_sifre{"-1"},_kid{} {}
loginpass::loginpass(const loginpass &kaynak):IdliSinif(kaynak),_sifre{kaynak.sifre()},_kid{kaynak.kid()} {}

QString loginpass::sifre() const
{
    return _sifre;
}

void loginpass::setSifre(const QString &newSifre)
{
    _sifre = newSifre;
}

quint32 loginpass::kid() const
{
    return _kid;
}

void loginpass::setKId(const quint32 &newKId)
{
    _kid = newKId;
}
