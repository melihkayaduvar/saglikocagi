#include "tetkik.h"

TETKIK::TETKIK():IdliSinif{},_ad{""},_aciklama{""},_normalDegerlerErkek{""},_normalDegerlerKadın{""} {}
TETKIK::TETKIK(const TETKIK &kaynak):IdliSinif{kaynak},_ad{kaynak._ad},_aciklama{kaynak._aciklama},_normalDegerlerErkek{kaynak._normalDegerlerErkek},_normalDegerlerKadın{kaynak._normalDegerlerKadın} {}


QString &TETKIK::ad()
{
    return _ad;
}

void TETKIK::setAd(const QString &newAd)
{
    _ad = newAd;
}

QString &TETKIK::aciklama()
{
    return _aciklama;
}

void TETKIK::setAciklama(const QString &newAciklama)
{
    _aciklama = newAciklama;
}

QString &TETKIK::normalDegerlerErkek()
{
    return _normalDegerlerErkek;
}

void TETKIK::setNormalDegerlerErkek(const QString &newNormalDeger)
{
    _normalDegerlerErkek = newNormalDeger;
}

QString TETKIK::normalDegerlerKadın()
{
    return _normalDegerlerKadın;
}

void TETKIK::setNormalDegerlerKadın(const QString &newNormalDeger)
{
    _normalDegerlerKadın = newNormalDeger;
}
