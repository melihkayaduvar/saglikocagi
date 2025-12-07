#include "istenentetkik.h"

ISTENENTETKIK::ISTENENTETKIK():IdliSinif(),_istekTarihi{},_sonucTarihi{},_sonuc{""},_yorum{""},_durum{},_ziyaretid{},_tetkikid{} {}
ISTENENTETKIK::ISTENENTETKIK(const ISTENENTETKIK& kaynak):IdliSinif(kaynak),_istekTarihi{kaynak._istekTarihi}
    ,_sonucTarihi{kaynak._sonucTarihi},_sonuc{kaynak._sonuc},_yorum{kaynak._yorum},_durum{kaynak._durum},_ziyaretid{kaynak._ziyaretid},_tetkikid{kaynak._tetkikid} {}

QDateTime &ISTENENTETKIK::istekTarihi()
{
    return _istekTarihi;
}

void ISTENENTETKIK::setIstekTarihi(const QDateTime &newIstekTarihi)
{
    _istekTarihi = newIstekTarihi;
}

QDateTime &ISTENENTETKIK::sonucTarihi()
{
    return _sonucTarihi;
}

void ISTENENTETKIK::setSonucTarihi(const QDateTime &newSonucTarihi)
{
    _sonucTarihi = newSonucTarihi;
}

QString &ISTENENTETKIK::sonuc()
{
    return _sonuc;
}

void ISTENENTETKIK::setSonuc(const QString &newSonuc)
{
    _sonuc = newSonuc;
}

QString &ISTENENTETKIK::yorum()
{
    return _yorum;
}

void ISTENENTETKIK::setYorum(const QString &newYorum)
{
    _yorum = newYorum;
}

TetkikDurumuEnum ISTENENTETKIK::durum() const
{
    return _durum;
}

void ISTENENTETKIK::setDurum(TetkikDurumuEnum newDurum)
{
    _durum = newDurum;
}

quint32 ISTENENTETKIK::ziyaretid() const
{
    return _ziyaretid;
}

void ISTENENTETKIK::setZiyaretId(const quint32 &newZiyaretId)
{
    _ziyaretid=newZiyaretId;
}

quint32 ISTENENTETKIK::tetkikid() const
{
    return _tetkikid;
}

void ISTENENTETKIK::setTetkikId(const quint32 &newTetkikId)
{
    _tetkikid=newTetkikId;
}

