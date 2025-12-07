#pragma once

#include "idlisinif.h"

class loginpass:public IdliSinif
{
private:
    loginpass();
    loginpass(const loginpass &kaynak);
    template <class T> friend class TABLO;
public:

    QString sifre() const;
    void setSifre(const QString &newSifre);

    quint32 kid() const;
    void setKId(const quint32 &newKId);

private:
    QString _sifre;
    quint32 _kid;
};
