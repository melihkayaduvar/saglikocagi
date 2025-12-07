#include "ilac.h"

ILAC::ILAC() {}
ILAC::ILAC(const ILAC& kaynak) {}

QString &ILAC::barkod()
{
    return _barkod;
}

void ILAC::setBarkod(const QString &newBarkod)
{
    _barkod = newBarkod;
}

QString &ILAC::ad()
{
    return _ad;
}

void ILAC::setAd(const QString &newAd)
{
    _ad = newAd;
}

QString &ILAC::etkenMadde()
{
    return _etkenMadde;
}

void ILAC::setEtkenMadde(const QString &newEtkenMadde)
{
    _etkenMadde = newEtkenMadde;
}

QString &ILAC::form()
{
    return _form;
}

void ILAC::setForm(const QString &newForm)
{
    _form = newForm;
}



QString &ILAC::dozajBilgisi()
{
    return _dozajBilgisi;
}

void ILAC::setDozajBilgisi(const QString &newDozajBilgisi)
{
    _dozajBilgisi = newDozajBilgisi;
}

