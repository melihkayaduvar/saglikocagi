#pragma once

#include "idlisinif.h"
#include "../Araclar/enums.h"

class BULGU:public IdliSinif
{
private:
    BULGU();
    BULGU(const BULGU& kaynak);
    template <class T> friend class TABLO;
public:
    QString &aciklama();
    void setAciklama(const QString &newAciklama);

    BulguTipiEnum tip() const;
    void setTip(BulguTipiEnum newTip);

    QDateTime &kaydedilmeZamani();
    void setKaydedilmeZamani(const QDateTime &newKaydedilmeZamani);

    quint32 ziyaretid()const;
    void setZiyaretId(const quint32 &newziyaretid);
private:
    QString _aciklama;
    BulguTipiEnum _tip;
    QDateTime _kaydedilmeZamani;

    //İlişkiler
    quint32 _ziyaretid;
};
