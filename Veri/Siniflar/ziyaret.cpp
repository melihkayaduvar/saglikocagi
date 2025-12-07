#include "ziyaret.h"

ZIYARET::ZIYARET():IdliSinif{},_tarihsaat{},_sikayet{""},_tani{""},_tedavinotlari{""},_doktorid{},_hastaid{} {}
ZIYARET::ZIYARET(const ZIYARET& kaynak):IdliSinif{kaynak},_tarihsaat{kaynak._tarihsaat},_sikayet{kaynak._sikayet}
    ,_tani{kaynak._tani},_tedavinotlari{kaynak._tedavinotlari},_doktorid{kaynak._doktorid},_hastaid{kaynak._hastaid} {}

QDateTime &ZIYARET::tarihsaat()
{
    return _tarihsaat;
}

void ZIYARET::setTarihsaat(const QDateTime &newTarihsaat)
{
    _tarihsaat = newTarihsaat;
}

QString &ZIYARET::sikayet()
{
    return _sikayet;
}

void ZIYARET::setSikayet(const QString &newSikayet)
{
    _sikayet = newSikayet;
}

QString &ZIYARET::tani()
{
    return _tani;
}

void ZIYARET::setTani(const QString &newTani)
{
    _tani = newTani;
}

QString &ZIYARET::tedavinotlari()
{
    return _tedavinotlari;
}

void ZIYARET::setTedavinotlari(const QString &newTedavinotlari)
{
    _tedavinotlari = newTedavinotlari;
}

quint32 ZIYARET::doktorid() const
{
    return _doktorid;
}

void ZIYARET::setDoktorId(const quint32 newDoktorid)
{
    _doktorid=newDoktorid;
}

quint32 ZIYARET::hastaid() const
{
    return _hastaid;
}

void ZIYARET::setHastaId(const quint32 newHastaid)
{
    _hastaid=newHastaid;
}
