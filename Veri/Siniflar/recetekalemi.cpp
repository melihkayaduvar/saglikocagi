#include "recetekalemi.h"

RECETEKALEMI::RECETEKALEMI():IdliSinif{},_kullanimSekli{""},_doz{""},_periyot{""},_adet{0},_receteid{},_ilacid{} {}
RECETEKALEMI::RECETEKALEMI(const RECETEKALEMI& kaynak):IdliSinif{kaynak},_kullanimSekli{kaynak._kullanimSekli}
    ,_doz{kaynak._doz},_periyot{kaynak._periyot},_adet{kaynak._adet},_receteid{kaynak._receteid},_ilacid{kaynak._ilacid} {}

QString &RECETEKALEMI::kullanimSekli()
{
    return _kullanimSekli;
}

void RECETEKALEMI::setKullanimSekli(const QString &newKullanimSekli)
{
    _kullanimSekli = newKullanimSekli;
}

QString &RECETEKALEMI::doz()
{
    return _doz;
}

void RECETEKALEMI::setDoz(const QString &newDoz)
{
    _doz = newDoz;
}

QString &RECETEKALEMI::periyot()
{
    return _periyot;
}

void RECETEKALEMI::setPeriyot(const QString &newPeriyot)
{
    _periyot = newPeriyot;
}

quint8 RECETEKALEMI::adet() const
{
    return _adet;
}

void RECETEKALEMI::setAdet(quint8 newAdet)
{
    _adet = newAdet;
}

quint32 RECETEKALEMI::receteid() const
{
    return _receteid;
}

void RECETEKALEMI::setReceteId(const quint32 &newReceteId)
{
    _receteid=newReceteId;
}

quint32 RECETEKALEMI::ilacid() const
{
    return _ilacid;
}

void RECETEKALEMI::setIlacID(quint32 newIlacID)
{
    _ilacid = newIlacID;
}
