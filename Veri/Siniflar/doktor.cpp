#include "doktor.h"

DOKTOR::DOKTOR():Insan{},_uzmanlikAlani{""} {}
DOKTOR::DOKTOR(const DOKTOR &kaynak):Insan{kaynak},_uzmanlikAlani{""} {}

QString &DOKTOR::diplomaNo()
{
    return _diplomaNo;
}

void DOKTOR::setdiplomano(const QString &newdiplomano)
{
    _diplomaNo=newdiplomano;
}

QString &DOKTOR::uzmanlikAlani()
{
    return _uzmanlikAlani;
}

void DOKTOR::setuzmanlikAlani(const QString &newuzmanlikalani)
{
    _uzmanlikAlani=newuzmanlikalani;
}
