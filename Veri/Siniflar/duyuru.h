#pragma once

#include "idlisinif.h"
class Duyuru:public IdliSinif
{
private:
    Duyuru();
    template <class T> friend class TABLO;
public:
    QString baslik() const;
    void setBaslik(const QString &newBaslik);

    QString metin() const;
    void setMetin(const QString &newMetin);

    QDate tarih() const;
    void setTarih(const QDate &newTarih);

private:
    QString _baslik;
    QString _metin;
    QDate _tarih;
};
