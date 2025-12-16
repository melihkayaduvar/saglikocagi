#pragma once

#include "Araclar/TABLO_TANIMLARI.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class VERITABANI
{
private:
    VERITABANI();
public:
    static VERITABANI& vt();

    DoktorTablosu& doktorlar();
    HastaTablosu& hastalar();
    ZiyaretTablosu& ziyaretler();
    BulguTablosu& bulgular();
    IstenenTetkikTablosu& istenentetkikler();
    ReceteTablosu& receteler();
    TetkikTablosu& tetkikler();
    IlacTablosu& ilaclar();
    ReceteKalemiTablosu& recetekalemleri();

    LoginPassTablosu& doktorsifreleri();
    LoginPassTablosu& yoneticisifreleri();

    DuyuruTablosu& duyurular();


    void baglan();
    void sqlAyarlariniYap();
    void ilkYukleme();

    quint32 aktifkid() const;
    void setAktifkid(quint32 newAktifkid);

private:
    QSqlDatabase db;

    DoktorTablosu tblDoktor;
    HastaTablosu tblHasta;
    ZiyaretTablosu tblZiyaret;
    BulguTablosu tblBulgu;
    IstenenTetkikTablosu tblIstenenTetkik;
    ReceteTablosu tblRecete;
    TetkikTablosu tblTetkik;
    IlacTablosu tblIlac;
    ReceteKalemiTablosu tblReceteKalemi;

    LoginPassTablosu tblDoktorPass;
    LoginPassTablosu tblYoneticiPass;

    DuyuruTablosu tblDuyuru;

    quint32 _aktifkid;
};
