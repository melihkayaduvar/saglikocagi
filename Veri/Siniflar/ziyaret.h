#pragma once

#include "idlisinif.h"

class ZIYARET:public IdliSinif{
public:
    ZIYARET();
    ZIYARET(const ZIYARET& kaynak);
    template <class T> friend class TABLO;

public:
    QDateTime &tarihsaat() ;
    void setTarihsaat(const QDateTime &newTarihsaat);

    QString &sikayet() ;
    void setSikayet(const QString &newSikayet);

    QString &tani();
    void setTani(const QString &newTani);

    QString &tedavinotlari() ;
    void setTedavinotlari(const QString &newTedavinotlari);

    quint32 doktorid() const;
    void setDoktorId(const quint32 newDoktorid);

    quint32 hastaid() const;
    void setHastaId(const quint32 newhastaid);





private:
    QDateTime _tarihsaat;
    QString _sikayet;
    QString _tani;
    QString _tedavinotlari;


    //İlişkiler
    quint32 _doktorid;
    quint32 _hastaid;

};
