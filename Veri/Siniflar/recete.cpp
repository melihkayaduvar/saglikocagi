#include "recete.h"

RECETE::RECETE():IdliSinif{},_tarih{},_gecerlilikSuresi{0},_ziyaretid{} {}
RECETE::RECETE(const RECETE& kaynak):IdliSinif{kaynak},_tarih{kaynak._tarih},_gecerlilikSuresi{kaynak._gecerlilikSuresi},_ziyaretid{kaynak._ziyaretid} {}

QDate &RECETE::tarih()
{
    return _tarih;
}

void RECETE::setTarih(const QDate &newTarih)
{
    _tarih = newTarih;
}

quint32 RECETE::gecerlilikSuresi() const
{
    return _gecerlilikSuresi;
}

void RECETE::setGecerlilikSuresi(quint32 newGecerlilikSuresi)
{
    _gecerlilikSuresi = newGecerlilikSuresi;
}

quint32 RECETE::ziyaretid() const
{
    return _ziyaretid;
}

void RECETE::setZiyaretId(const quint32 &newziyaretid)
{
    _ziyaretid=newziyaretid;
}
