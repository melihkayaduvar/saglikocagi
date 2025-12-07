#include "duyuru.h"

Duyuru::Duyuru() {}

QString Duyuru::baslik() const
{
    return _baslik;
}

void Duyuru::setBaslik(const QString &newBaslik)
{
    _baslik = newBaslik;
}

QString Duyuru::metin() const
{
    return _metin;
}

void Duyuru::setMetin(const QString &newMetin)
{
    _metin = newMetin;
}

QDate Duyuru::tarih() const
{
    return _tarih;
}

void Duyuru::setTarih(const QDate &newTarih)
{
    _tarih = newTarih;
}
