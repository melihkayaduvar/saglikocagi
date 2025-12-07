#pragma once

#include "idlisinif.h"

class TETKIK:public IdliSinif
{
private:
    TETKIK();
    TETKIK(const TETKIK& kaynak);
    template <class T> friend class TABLO;
public:
    QString &ad() ;
    void setAd(const QString &newAd);

    QString &aciklama() ;
    void setAciklama(const QString &newAciklama);

    QString &normalDegerlerErkek() ;
    void setNormalDegerlerErkek(const QString &newNormalDeger);

    QString normalDegerlerKadın();
    void setNormalDegerlerKadın(const QString &newNormalDeger);

private:
    QString _ad;
    QString _aciklama;
    QString _normalDegerlerErkek;
    QString _normalDegerlerKadın;
};
