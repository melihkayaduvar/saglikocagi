#include "insan.h"

Insan::Insan():IdliSinif{},_adi{""},_soyadi{""},_telefon{""} {}
Insan::Insan(const Insan &kaynak):IdliSinif{kaynak},_adi{kaynak._adi},_soyadi{kaynak._soyadi},_telefon{kaynak._telefon} {}

QString &Insan::adi()
{
    return _adi;
}

void Insan::setadi(const QString &newadi)
{
    _adi=newadi;
}

QString &Insan::soyadi()
{
    return _soyadi;
}

void Insan::setsoyadi(const QString &newsoyadi)
{
    _soyadi=newsoyadi;
}

QString &Insan::telefon()
{
    return _telefon;
}

void Insan::settelefon(const QString &newtelefon)
{
    _telefon=newtelefon;
}
