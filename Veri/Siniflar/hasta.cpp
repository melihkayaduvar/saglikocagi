#include "hasta.h"

HASTA::HASTA():Insan(),_tckimlik{""},_dogumTarihi{},
    _adres{""},_kanGrubu{""},_alerjiler{""},_kronikHastaliklar{""},_cinsiyet{CinsiyetEnum::BELIRTILMEMIS} {}
HASTA::HASTA(const HASTA& kaynak):Insan(kaynak),_tckimlik{kaynak._tckimlik},_dogumTarihi{kaynak._dogumTarihi},
    _adres{kaynak._adres},_kanGrubu{kaynak._kanGrubu},_alerjiler{kaynak._alerjiler},_kronikHastaliklar{kaynak._kronikHastaliklar},_cinsiyet{kaynak._cinsiyet} {}

QString &HASTA::tckimlik()
{
    return _tckimlik;
}

void HASTA::setTckimlik(const QString &newTckimlik)
{
    _tckimlik = newTckimlik;
}

QDate &HASTA::dogumTarihi()
{
    return _dogumTarihi;
}

void HASTA::setDogumTarihi(const QDate &newDogumTarihi)
{
    _dogumTarihi = newDogumTarihi;
}

QString &HASTA::adres()
{
    return _adres;
}

void HASTA::setAdres(const QString &newAdres)
{
    _adres = newAdres;
}

QString &HASTA::kanGrubu()
{
    return _kanGrubu;
}

void HASTA::setKanGrubu(const QString &newKanGrubu)
{
    _kanGrubu = newKanGrubu;
}

QStringList &HASTA::alerjiler()
{
    return _alerjiler;
}

void HASTA::setAlerjiler(const QStringList &newAlerjiler)
{
    _alerjiler = newAlerjiler;
}

QStringList &HASTA::kronikHastaliklar()
{
    return _kronikHastaliklar;
}

void HASTA::setKronikHastaliklar(const QStringList &newKronikHastaliklar)
{
    _kronikHastaliklar = newKronikHastaliklar;
}

CinsiyetEnum HASTA::cinsiyet() const
{
    return _cinsiyet;
}

void HASTA::setCinsiyet(CinsiyetEnum newCinsiyet)
{
    _cinsiyet = newCinsiyet;
}
