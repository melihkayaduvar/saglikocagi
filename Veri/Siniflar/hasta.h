#pragma once

#include "insan.h"
#include "../Araclar/enums.h"

class HASTA:public Insan{
private:
    HASTA();
    HASTA(const HASTA& kaynak);
    template <class T> friend class TABLO;
public:
    QString &tckimlik() ;
    void setTckimlik(const QString &newTckimlik);

    QDate &dogumTarihi() ;
    void setDogumTarihi(const QDate &newDogumTarihi);

    QString &adres() ;
    void setAdres(const QString &newAdres);

    QString &kanGrubu() ;
    void setKanGrubu(const QString &newKanGrubu);

    QStringList &alerjiler() ;
    void setAlerjiler(const QStringList &newAlerjiler);

    QStringList &kronikHastaliklar() ;
    void setKronikHastaliklar(const QStringList &newKronikHastaliklar);

    CinsiyetEnum cinsiyet() const;
    void setCinsiyet(CinsiyetEnum newCinsiyet);


private:
    QString _tckimlik;
    QDate _dogumTarihi;
    CinsiyetEnum _cinsiyet;
    QString _adres;
    QString _kanGrubu;
    QStringList _alerjiler;
    QStringList _kronikHastaliklar;
};
