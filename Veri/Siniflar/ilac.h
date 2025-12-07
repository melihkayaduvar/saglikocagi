#pragma once

#include "idlisinif.h"

class ILAC:public IdliSinif
{
private:
    ILAC();
    ILAC(const ILAC& kaynak);
    template <class T> friend class TABLO;
public:

    QString &barkod() ;
    void setBarkod(const QString &newBarkod);

    QString &ad() ;
    void setAd(const QString &newAd);

    QString &etkenMadde() ;
    void setEtkenMadde(const QString &newEtkenMadde);

    QString &form() ;
    void setForm(const QString &newForm);

    QString &dozajBilgisi() ;
    void setDozajBilgisi(const QString &newDozajBilgisi);

private:
    QString _barkod;
    QString _ad;
    QString _etkenMadde;
    QString _form;
    QString _dozajBilgisi;
};
