#include "bulgu.h"

BULGU::BULGU():IdliSinif{},_aciklama{""},_tip{},_kaydedilmeZamani{},_ziyaretid{} {}
BULGU::BULGU(const BULGU& kaynak):IdliSinif{kaynak},_aciklama{kaynak._aciklama},_tip{kaynak._tip},_kaydedilmeZamani{kaynak._kaydedilmeZamani},_ziyaretid{kaynak._ziyaretid} {}

QString &BULGU::aciklama()
{
    return _aciklama;
}

void BULGU::setAciklama(const QString &newAciklama)
{
    _aciklama = newAciklama;
}

BulguTipiEnum BULGU::tip() const
{
    return _tip;
}

void BULGU::setTip(BulguTipiEnum newTip)
{
    _tip = newTip;
}

QDateTime &BULGU::kaydedilmeZamani()
{
    return _kaydedilmeZamani;
}

void BULGU::setKaydedilmeZamani(const QDateTime &newKaydedilmeZamani)
{
    _kaydedilmeZamani = newKaydedilmeZamani;
}

quint32 BULGU::ziyaretid() const
{
    return _ziyaretid;
}

void BULGU::setZiyaretId(const quint32 &newziyaretid)
{
    _ziyaretid=newziyaretid;
}














/*QDataStream &operator<<(QDataStream &out, const BULGU &z)
{
    out << z._aciklama
        << z._tip
        << z._kaydedilmeZamani;

    return out;
}

QDataStream &operator>>(QDataStream &in, BULGU &z)
{
    in >> z._aciklama
        >> z._tip
        >> z._kaydedilmeZamani;


    return in;
}*/
