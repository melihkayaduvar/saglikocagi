 #pragma once

#include "insan.h"

class DOKTOR:public Insan{
private:
    DOKTOR();
    DOKTOR(const DOKTOR& kaynak);
    template <class T> friend class TABLO;
public:
    QString &diplomaNo();
    void setdiplomano(const QString &newdiplomano);

    QString &uzmanlikAlani();
    void setuzmanlikAlani(const QString &newuzmanlikalani);
private:
    QString _diplomaNo;
    QString _uzmanlikAlani;
};

