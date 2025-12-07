#pragma once

#include "idlisinif.h"
#include "../Araclar/enums.h"

class ISTENENTETKIK:public IdliSinif
{
private:
    ISTENENTETKIK();
    ISTENENTETKIK(const ISTENENTETKIK& kaynak);
    template <class T> friend class TABLO;
public:

    QDateTime &istekTarihi();
    void setIstekTarihi(const QDateTime &newIstekTarihi);

    QDateTime &sonucTarihi();
    void setSonucTarihi(const QDateTime &newSonucTarihi);

    QString &sonuc();
    void setSonuc(const QString &newSonuc);

    QString &yorum();
    void setYorum(const QString &newYorum);

    TetkikDurumuEnum durum()const;
    void setDurum(TetkikDurumuEnum newDurum);

    quint32 ziyaretid()const;
    void setZiyaretId(const quint32 &newZiyaretId);

    quint32 tetkikid()const;
    void setTetkikId(const quint32 &newTetkikId);

private:
    QDateTime _istekTarihi;
    QDateTime _sonucTarihi;
    QString _sonuc;
    QString _yorum;
    TetkikDurumuEnum _durum;


    //İlişkiler
    quint32 _ziyaretid;
    quint32 _tetkikid;
};
