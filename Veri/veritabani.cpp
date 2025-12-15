#include "veritabani.h"
#include <QMessageBox>

VERITABANI::VERITABANI() {
    baglan();
    sqlAyarlariniYap();
    ilkYukleme();
}

VERITABANI &VERITABANI::vt()
{
    static VERITABANI nesne;
    return nesne;
}

void VERITABANI::baglan()
{
    db = QSqlDatabase::addDatabase("QODBC");
    // SSMS'teki sunucu adını buraya yaz (.\SQLEXPRESS veya localhost)
    QString serverName = "localhost\\SQLEXPRESS";
    QString dbName = "SaglikOcagiDB";

    QString dsn = QString("DRIVER={ODBC Driver 17 for SQL Server};SERVER=%1;DATABASE=%2;Trusted_Connection=yes;")
                      .arg(serverName,dbName);

    db.setDatabaseName(dsn);

    if (!db.open()) {
        qDebug() << "Veritabanı Bağlantı Hatası:" << db.lastError().text();
    }
}

// TABLOLAR İÇİN EKLEME VE SİLME KURALLARI (SQL INSERT/DELETE)
void VERITABANI::sqlAyarlariniYap()
{
    // --- DOKTORLAR ---
    tblDoktor.setSqlEklemeIslemi([this](DoktorTablosu::VeriPointer d){
        QSqlQuery q;
        q.prepare("INSERT INTO doktorlar (ad, soyad, telefon, diploma_no, uzmanlik) VALUES (?, ?, ?, ?, ?)");
        q.addBindValue(d->adi());
        q.addBindValue(d->soyadi());
        q.addBindValue(d->telefon());
        q.addBindValue(d->diplomaNo());
        q.addBindValue(d->uzmanlikAlani());
        if(q.exec()){
            d->setId(q.lastInsertId().toInt()); // SQL'den gelen ID'yi nesneye ata
        } else { qDebug() << "Doktor Ekleme Hatası:" << q.lastError().text(); }
    });

    tblDoktor.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM doktorlar WHERE id=?"); q.addBindValue(id); q.exec();
        q.prepare("DELETE FROM doktorpass WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblDoktor.setSqlGuncellemeIslemi([](DoktorTablosu::VeriPointer d){
        QSqlQuery q;
        q.prepare("UPDATE doktorlar SET ad=?, soyad=?, telefon=?, diploma_no=?, uzmanlik=? WHERE id=?");
        q.addBindValue(d->adi());
        q.addBindValue(d->soyadi());
        q.addBindValue(d->telefon());
        q.addBindValue(d->diplomaNo());
        q.addBindValue(d->uzmanlikAlani());
        q.addBindValue(d->id()); // WHERE koşulu için ID en sonda
        if(!q.exec()) qDebug() << "Doktor Güncelleme Hatası:" << q.lastError().text();
    });

    // --- HASTALAR ---
    tblHasta.setSqlEklemeIslemi([this](HastaTablosu::VeriPointer h){
        QSqlQuery q;
        // Not: StringList olan alerjiler ve kronik hastalıkları join ile birleştirip kaydediyoruz
        q.prepare("INSERT INTO hastalar (tc_no, ad, soyad, telefon, dogum_tarihi, cinsiyet, adres, kan_grubu) VALUES (?,?,?,?,?,?,?,?,)");
        q.addBindValue(h->tckimlik());
        q.addBindValue(h->adi());
        q.addBindValue(h->soyadi());
        q.addBindValue(h->telefon());
        q.addBindValue(h->dogumTarihi());
        q.addBindValue(static_cast<int>(h->cinsiyet())); // Enum int olarak kaydedilir
        q.addBindValue(h->adres());
        q.addBindValue(h->kanGrubu());

        if(q.exec()) {
            int yeniHastaid = q.lastInsertId().toInt();
            h->setId(yeniHastaid);

            QSqlQuery qAlt;
            qAlt.prepare("INSERT INTO hasta_alerjiler (hasta_id, alerji_adi) VALUES (?,?)");
            for(const QString &alerji:h->alerjiler()){
                qAlt.bindValue(0, yeniHastaid);
                qAlt.bindValue(1, alerji);
                qAlt.exec();
            }
            qAlt.prepare("INSERT INTO hasta_kronik_hastaliklar (hasta_id, hastalik_adi) VALUES (?,?)");
            for(const QString &hastalik:h->kronikHastaliklar()){
                qAlt.bindValue(0, yeniHastaid);
                qAlt.bindValue(1, hastalik);
                qAlt.exec();
            }
        }
        else qDebug() << "Hasta Ekleme Hatası:" << q.lastError().text();
    });

    tblHasta.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q;
        q.prepare("DELETE FROM hasta_alerjiler WHERE hasta_id=?"); q.addBindValue(id); q.exec();
        q.prepare("DELETE FROM hasta_kronik_hastaliklar WHERE hasta_id=?"); q.addBindValue(id); q.exec();
        q.prepare("DELETE FROM hastalar WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblHasta.setSqlGuncellemeIslemi([](HastaTablosu::VeriPointer h){
        QSqlQuery q;
        q.prepare("UPDATE hastalar SET tc_no=?, ad=?, soyad=?, telefon=?, dogum_tarihi=?, cinsiyet=?, adres=?, kan_grubu=? WHERE id=?");
        q.addBindValue(h->tckimlik());
        q.addBindValue(h->adi());
        q.addBindValue(h->soyadi());
        q.addBindValue(h->telefon());
        q.addBindValue(h->dogumTarihi());
        q.addBindValue(static_cast<int>(h->cinsiyet()));
        q.addBindValue(h->adres());
        q.addBindValue(h->kanGrubu());
        q.addBindValue(h->id());
        if(!q.exec()) {
            qDebug() << "Hasta Güncelleme Hatası: " << q.lastError().text();
            return;
        }
        QSqlQuery qSil;
        qSil.prepare("DELETE FROM hasta_alerjiler WHERE hasta_id=?");
        qSil.addBindValue(h->id());
        qSil.exec();
        qSil.prepare("DELETE FROM hasta_kronik_hastaliklar WHERE hasta_id=?");
        qSil.addBindValue(h->id());
        qSil.exec();
        QSqlQuery qEkle;
        qEkle.prepare("INSERT INTO hasta_alerjiler (hasta_id, alerji_adi) VALUES (?,?)");
        for(const QString &alerji:h->alerjiler()){
            qEkle.bindValue(0,h->id());
            qEkle.bindValue(1,alerji);
            qEkle.exec();
        }
        qEkle.prepare("INSERT INTO hasta_kronik_hastaliklar (hasta_id, hastalik_adi) VALUES (?,?)");
        for(const QString &hastalik:h->kronikHastaliklar()){
            qEkle.bindValue(0,h->id());
            qEkle.bindValue(1,hastalik);
            qEkle.exec();
        }
    });

    tblBulgu.setSqlEklemeIslemi([this](BulguTablosu::VeriPointer b){
        QSqlQuery q;
        q.prepare("INSERT INTO bulgular (aciklama,tip,kaydedilmezamani,ziyaretid) VALUES (?,?,?,?)");
        q.addBindValue(b->aciklama());
        q.addBindValue(static_cast<int>(b->tip()));
        q.addBindValue(b->kaydedilmeZamani());
        q.addBindValue(b->ziyaretid());

        if(q.exec()) b->setId(q.lastInsertId().toInt());
        else qDebug() << "Bulgu Ekleme Hatası: " << q.lastError().text();
    });

    tblBulgu.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM bulgular WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblBulgu.setSqlGuncellemeIslemi([](BulguTablosu::VeriPointer b){
        QSqlQuery q;
        q.prepare("UPDATE bulgular SET aciklama=?, tip=?, kaydedilmezamani=?, ziyaretid=? WHERE id=?");
        q.addBindValue(b->aciklama());
        q.addBindValue(static_cast<int>(b->tip()));
        q.addBindValue(b->kaydedilmeZamani());
        q.addBindValue(b->ziyaretid());
        q.addBindValue(b->id());
        if(!q.exec()) qDebug() << "Bulgu Güncelleme Hatası:" << q.lastError().text();
    });

    tblIstenenTetkik.setSqlEklemeIslemi([this](IstenenTetkikTablosu::VeriPointer i){
        QSqlQuery q; q.prepare("INSERT INTO istenentetkikler (istektarihi,sonuctarihi,sonuc,yorum,durum,ziyaretid,tetkikid) VALUES (?,?,?,?,?,?,?)");
        q.addBindValue(i->istekTarihi());
        q.addBindValue(i->sonucTarihi());
        q.addBindValue(i->sonuc());
        q.addBindValue(i->yorum());
        q.addBindValue(static_cast<int>(i->durum()));
        q.addBindValue(i->ziyaretid());
        q.addBindValue(i->tetkikid());

        if(q.exec()) i->setId(q.lastInsertId().toInt());
        else qDebug() << "İstenen Tetkik Ekleme Hatası: " << q.lastError().text();
    });

    tblIstenenTetkik.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM istenentetkikler WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblIstenenTetkik.setSqlGuncellemeIslemi([](IstenenTetkikTablosu::VeriPointer i){
        QSqlQuery q;
        q.prepare("UPDATE istenentetkikler SET istektarihi=?, sonuctarihi=?, sonuc=?, yorum=?, durum=?, ziyaretid=?, tetkikid=? WHERE id=?");
        q.addBindValue(i->istekTarihi());
        q.addBindValue(i->sonucTarihi());
        q.addBindValue(i->sonuc());
        q.addBindValue(i->yorum());
        q.addBindValue(static_cast<int>(i->durum()));
        q.addBindValue(i->ziyaretid());
        q.addBindValue(i->tetkikid());
        q.addBindValue(i->id());
        if(!q.exec()) qDebug() << "Tetkik Sonuç Güncelleme Hatası:" << q.lastError().text();
    });

    tblZiyaret.setSqlEklemeIslemi([this](ZiyaretTablosu::VeriPointer z){
        QSqlQuery q; q.prepare("INSERT INTO ziyaretler (tarihsaat,sikayet,tani,tedavinotlari,doktorid,hastaid) VALUES (?,?,?,?,?,?)");
        q.addBindValue(z->tarihsaat());
        q.addBindValue(z->sikayet());
        q.addBindValue(z->tani());
        q.addBindValue(z->tedavinotlari());
        q.addBindValue(z->doktorid());
        q.addBindValue(z->hastaid());

        if(q.exec()) z->setId(q.lastInsertId().toInt());
        else qDebug() << "Ziyaret Ekleme Hatası: ";
    });

    tblZiyaret.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM ziyaretler WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblZiyaret.setSqlGuncellemeIslemi([](ZiyaretTablosu::VeriPointer z){
        QSqlQuery q;
        q.prepare("UPDATE ziyaretler SET tarihsaat=?, sikayet=?, tani=?, tedavinotlari=?, doktorid=?, hastaid=? WHERE id=?");
        q.addBindValue(z->tarihsaat());
        q.addBindValue(z->sikayet());
        q.addBindValue(z->tani());
        q.addBindValue(z->tedavinotlari());
        q.addBindValue(z->doktorid());
        q.addBindValue(z->hastaid());
        q.addBindValue(z->id());
        if(!q.exec()) qDebug() << "Ziyaret Güncelleme Hatası:" << q.lastError().text();
    });

    tblRecete.setSqlEklemeIslemi([this](ReceteTablosu::VeriPointer r){
        QSqlQuery q; q.prepare("INSERT INTO receteler (tarih,gecerliliksuresi,ziyaretid) VALUES (?,?,?)");
        q.addBindValue(r->tarih());
        q.addBindValue(r->gecerlilikSuresi());
        q.addBindValue(r->ziyaretid());

        if(q.exec()) r->setId(q.lastInsertId().toInt());
        else qDebug() << "Recete Ekleme Hatası: ";
    });

    tblRecete.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM receteler WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblRecete.setSqlGuncellemeIslemi([](ReceteTablosu::VeriPointer r){
        QSqlQuery q;
        q.prepare("UPDATE receteler SET tarih=?, gecerliliksuresi=?, ziyaretid=? WHERE id=?");
        q.addBindValue(r->tarih());
        q.addBindValue(r->gecerlilikSuresi());
        q.addBindValue(r->ziyaretid());
        q.addBindValue(r->id());
        if(!q.exec()) qDebug() << "Reçete Güncelleme Hatası:" << q.lastError().text();
    });

    tblTetkik.setSqlEklemeIslemi([this](TetkikTablosu::VeriPointer t){
        QSqlQuery q; q.prepare("INSERT INTO tetkikler (ad,aciklama,nderkek,ndkadin) VALUES (?,?,?,?)");
        q.addBindValue(t->ad());
        q.addBindValue(t->aciklama());
        q.addBindValue(t->normalDegerlerErkek());
        q.addBindValue(t->normalDegerlerKadın());

        if(q.exec()) t->setId(q.lastInsertId().toInt());
        else qDebug() << "Tetkik Ekleme Hatası: ";
    });

    tblTetkik.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM tetkikler WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblTetkik.setSqlGuncellemeIslemi([](TetkikTablosu::VeriPointer t){
        QSqlQuery q;
        q.prepare("UPDATE tetkikler SET ad=?, aciklama=?, nderkek=?, ndkadin=? WHERE id=?");
        q.addBindValue(t->ad());
        q.addBindValue(t->aciklama());
        q.addBindValue(t->normalDegerlerErkek());
        q.addBindValue(t->normalDegerlerKadın());
        q.addBindValue(t->id());

        if(!q.exec()) qDebug() << "Tetkik Güncelleme Hatası: " << q.lastError().text();
    });

    tblIlac.setSqlEklemeIslemi([this](IlacTablosu::VeriPointer i){
        QSqlQuery q; q.prepare("INSERT INTO ilaclar (barkod,ad,etkenmadde,form,dozajbilgisi) VALUES (?,?,?,?,?)");
        q.addBindValue(i->barkod());
        q.addBindValue(i->ad());
        q.addBindValue(i->etkenMadde());
        q.addBindValue(i->form());
        q.addBindValue(i->dozajBilgisi());

        if(q.exec()) i->setId(q.lastInsertId().toInt());
        else qDebug() << "Ilac Ekleme Hatası: ";
    });

    tblIlac.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM ilaclar WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblIlac.setSqlGuncellemeIslemi([](IlacTablosu::VeriPointer i){
        QSqlQuery q;
        q.prepare("UPDATE ilaclar SET barkod=?, ad=?, etkenmadde=?, form=?, dozajbilgisi=? WHERE id=?");
        q.addBindValue(i->barkod());
        q.addBindValue(i->ad());
        q.addBindValue(i->etkenMadde());
        q.addBindValue(i->form());
        q.addBindValue(i->dozajBilgisi());
        q.addBindValue(i->id());
        if(!q.exec()) qDebug() << "İlaç Güncelleme Hatası:" << q.lastError().text();
    });

    tblReceteKalemi.setSqlEklemeIslemi([this](ReceteKalemiTablosu::VeriPointer r){
        QSqlQuery q; q.prepare("INSERT INTO recetekalemleri (kullanimsekli,doz,periyot,adet,receteid,ilacid) VALUES (?,?,?,?,?,?)");
        q.addBindValue(r->kullanimSekli());
        q.addBindValue(r->doz());
        q.addBindValue(r->periyot());
        q.addBindValue(r->adet());
        q.addBindValue(r->receteid());
        q.addBindValue(r->ilacid());

        if(q.exec()) r->setId(q.lastInsertId().toInt());
        else qDebug() << "Recete Kalemi Ekleme Hatası: ";
    });

    tblReceteKalemi.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM recetekalemleri WHERE id=?"); q.addBindValue(id); q.exec();
    });

    tblReceteKalemi.setSqlGuncellemeIslemi([](ReceteKalemiTablosu::VeriPointer r){
        QSqlQuery q;
        q.prepare("UPDATE recetekalemleri SET kullanimsekli=?, doz=?, periyot=?, adet=?, receteid=?, ilacid=? WHERE id=?");
        q.addBindValue(r->kullanimSekli());
        q.addBindValue(r->doz());
        q.addBindValue(r->periyot());
        q.addBindValue(r->adet());
        q.addBindValue(r->receteid());
        q.addBindValue(r->ilacid());
        q.addBindValue(r->id());
        if(!q.exec()) qDebug() << "Reçete Kalemi Güncelleme Hatası: " << q.lastError().text();
    });

    tblDuyuru.setSqlEklemeIslemi([this](DuyuruTablosu::VeriPointer d){
        QSqlQuery q; q.prepare("INSERT INTO duyurular (baslik,metin,tarih) VALUES (?,?,?)");
        q.addBindValue(d->baslik());
        q.addBindValue(d->metin());
        q.addBindValue(d->tarih());

        if(q.exec()) d->setId(q.lastInsertId().toInt());
        else qDebug() << "Duyuru Ekleme Hatası: ";
    });

    tblDuyuru.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM duyurular WHERE id=?"); q.addBindValue(id);q.exec();
    });

    tblDuyuru.setSqlGuncellemeIslemi([](DuyuruTablosu::VeriPointer d){
        QSqlQuery q;
        q.prepare("UPDATE duyurular SET baslik=?, metin=?, tarih=? WHERE id=?");
        q.addBindValue(d->baslik());
        q.addBindValue(d->metin());
        q.addBindValue(d->tarih());
        q.addBindValue(d->id());
        if(!q.exec()) qDebug() << "Duyuru Güncelleme Hatası:" << q.lastError().text();
    });

    tblDoktorPass.setSqlEklemeIslemi([this](LoginPassTablosu::VeriPointer d){
        QSqlQuery q; q.prepare("INSERT INTO doktorpass (kid,sifre) VALUES (?,?)");
        q.addBindValue(d->kid());
        q.addBindValue(d->sifre());

        if(q.exec()) d->setId(q.lastInsertId().toInt());
        else qDebug() << "DoktorPass Ekleme Hatası: ";
    });

    tblDoktorPass.setSqlGuncellemeIslemi([](LoginPassTablosu::VeriPointer d){
        QSqlQuery q;
        q.prepare("UPDATE doktorpass SET kid=?, sifre=? WHERE id=?");
        q.addBindValue(d->kid());
        q.addBindValue(d->sifre());
        q.addBindValue(d->id());
        q.exec();
    });

    tblYoneticiPass.setSqlEklemeIslemi([this](LoginPassTablosu::VeriPointer d){
        QSqlQuery q; q.prepare("INSERT INTO yoneticipass (kid,sifre) VALUES (?,?)");
        q.addBindValue(d->kid());
        q.addBindValue(d->sifre());

        if(q.exec()) d->setId(q.lastInsertId().toInt());
        else qDebug() << "YöneticiPass Ekleme Hatası: ";
    });

    tblYoneticiPass.setSqlSilmeIslemi([this](quint32 id){
        QSqlQuery q; q.prepare("DELETE FROM yoneticipass WHERE VALUES (?,?)");
    });

    tblYoneticiPass.setSqlGuncellemeIslemi([](LoginPassTablosu::VeriPointer d){
        QSqlQuery q;
        q.prepare("UPDATE yoneticipass SET kid=?, sifre=? WHERE id=?");
        q.addBindValue(d->kid());
        q.addBindValue(d->sifre());
        q.addBindValue(d->id());
        q.exec();
    });
}

void VERITABANI::ilkYukleme()
{
    QSqlQuery qDoktor("SELECT * FROM doktorlar");
    while(qDoktor.next()){
        auto d = tblDoktor.olustur();
        d->setId(qDoktor.value("id").toInt());
        d->setadi(qDoktor.value("ad").toString());
        d->setsoyadi(qDoktor.value("soyad").toString());
        d->settelefon(qDoktor.value("telefon").toString());
        d->setdiplomano(qDoktor.value("diploma_no").toString());
        d->setuzmanlikAlani(qDoktor.value("uzmanlik").toString());
        tblDoktor.ramEkle(d);
    }

    QSqlQuery qHasta("SELECT * FROM hastalar");
    while(qHasta.next()){
        auto h = tblHasta.olustur();
        h->setId(qHasta.value("id").toInt());
        h->setTckimlik(qHasta.value("tc_no").toString());
        h->setadi(qHasta.value("ad").toString());
        h->setsoyadi(qHasta.value("soyad").toString());
        h->settelefon(qHasta.value("telefon").toString());
        h->setDogumTarihi(qHasta.value("dogum_tarihi").toDate());
        h->setCinsiyet((CinsiyetEnum)qHasta.value("cinsiyet").toInt());
        h->setAdres(qHasta.value("adres").toString());
        h->setKanGrubu(qHasta.value("kan_grubu").toString());
        QStringList alerjiListesi;
        QSqlQuery qAlerji;
        qAlerji.prepare("SELECT alerji_adi FROM hasta_alerjiler WHERE hasta_id=?");
        qAlerji.addBindValue(h->id());
        if(qAlerji.exec()){
            while (qAlerji.next()) {
                alerjiListesi.append(qAlerji.value(0).toString());
            }
        }

        h->setAlerjiler(alerjiListesi);

        QStringList khastalikListesi;
        QSqlQuery qKhastalik;
        qKhastalik.prepare("SELECT hastalik_adi FROM hasta_kronik_hastaliklar WHERE hasta_id=?");
        qKhastalik.addBindValue(h->id());
        if(qKhastalik.exec()){
            while (qKhastalik.next()) {
                khastalikListesi.append(qKhastalik.value(0).toString());
            }
        }
        h->setKronikHastaliklar(khastalikListesi);
        tblHasta.ramEkle(h);
    }

    QSqlQuery qBulgu("SELECT * FROM bulgular");
    while(qBulgu.next()){
        auto b = tblBulgu.olustur();
        b->setId(qBulgu.value("id").toInt());
        b->setAciklama(qBulgu.value("aciklama").toString());
        b->setTip((BulguTipiEnum)qBulgu.value("tipi").toInt());
        b->setKaydedilmeZamani(qBulgu.value("kaydedilmezamani").toDateTime());
        b->setZiyaretId(qBulgu.value("ziyaretid").toInt());

        tblBulgu.ramEkle(b);
    }

    QSqlQuery qIstenenTetkik("SELECT * FROM istenentetkikler");
    while(qIstenenTetkik.next()){
        auto i = tblIstenenTetkik.olustur();
        i->setId(qIstenenTetkik.value("id").toInt());
        i->setIstekTarihi(qIstenenTetkik.value("istektarihi").toDateTime());
        i->setSonucTarihi(qIstenenTetkik.value("sonuctarihi").toDateTime());
        i->setSonuc(qIstenenTetkik.value("sonuc").toString());
        i->setYorum(qIstenenTetkik.value("yorum").toString());
        i->setZiyaretId(qIstenenTetkik.value("ziyaretid").toInt());
        i->setTetkikId(qIstenenTetkik.value("tetkikid").toInt());

        tblIstenenTetkik.ramEkle(i);
    }

    QSqlQuery qZiyaret("SELECT * FROM ziyaretler");
    while(qZiyaret.next()){
        auto z = tblZiyaret.olustur();
        z->setId(qZiyaret.value("id").toInt());
        z->setTarihsaat(qZiyaret.value("tarihsaat").toDateTime());
        z->setSikayet(qZiyaret.value("sikayet").toString());
        z->setTani(qZiyaret.value("ziyaret").toString());
        z->setTedavinotlari(qZiyaret.value("tedavinot").toString());
        z->setDoktorId(qZiyaret.value("doktorid").toInt());
        z->setHastaId(qZiyaret.value("hastaid").toInt());

        tblZiyaret.ramEkle(z);
    }

    QSqlQuery qRecete("SELECT * FROM receteler");
    while (qRecete.next()) {
        auto r = tblRecete.olustur();
        r->setId(qRecete.value("id").toInt());
        r->setTarih(qRecete.value("tarih").toDate());
        r->setGecerlilikSuresi(qRecete.value("gecerliliksuresi").toInt());
        r->setZiyaretId(qRecete.value("ziyaretid").toInt());

        tblRecete.ramEkle(r);
    }

    QSqlQuery qTetkik("SELECT * FROM tetkikler");
    while (qTetkik.next()) {
        auto t=tblTetkik.olustur();
        t->setId(qTetkik.value("id").toInt());
        t->setAd(qTetkik.value("ad").toString());
        t->setAciklama(qTetkik.value("aciklama").toString());
        t->setNormalDegerlerErkek(qTetkik.value("nderkek").toString());
        t->setNormalDegerlerKadın(qTetkik.value("ndkadin").toString());

        tblTetkik.ramEkle(t);
    }

    QSqlQuery qIlac("SELECT * FROM ilaclar");
    while (qIlac.next()) {
        auto i = tblIlac.olustur();
        i->setId(qIlac.value("id").toInt());
        i->setBarkod(qIlac.value("barkod").toString());
        i->setAd(qIlac.value("ad").toString());
        i->setEtkenMadde(qIlac.value("etkenmadde").toString());
        i->setForm(qIlac.value("form").toString());
        i->setDozajBilgisi(qIlac.value("dozajbilgisi").toString());

        tblIlac.ramEkle(i);
    }

    QSqlQuery qReceteKalemi("SELECT * FROM recetekalemleri");
    while (qReceteKalemi.next()) {
        auto r = tblReceteKalemi.olustur();
        r->setId(qReceteKalemi.value("id").toInt());
        r->setKullanimSekli(qReceteKalemi.value("kullanimsekli").toString());
        r->setDoz(qReceteKalemi.value("doz").toString());
        r->setPeriyot(qReceteKalemi.value("periyot").toString());
        r->setAdet(qReceteKalemi.value("adet").toInt());
        r->setReceteId(qReceteKalemi.value("receteid").toInt());
        r->setIlacID(qReceteKalemi.value("ilacid").toInt());

        tblReceteKalemi.ramEkle(r);
    }

    QSqlQuery qDuyuru("SELECT * FROM duyurular");
    while (qDuyuru.next()) {
        auto d = tblDuyuru.olustur();
        d->setId(qDuyuru.value("id").toInt());
        d->setBaslik(qDuyuru.value("baslik").toString());
        d->setMetin(qDuyuru.value("metin").toString());
        d->setTarih(qDuyuru.value("tarih").toDate());

        tblDuyuru.ramEkle(d);
    }

    QSqlQuery qDoktorPass("SELECT * FROM doktorpass");
    while (qDoktorPass.next()) {
        auto dp=tblDoktorPass.olustur();
        dp->setId(qDoktorPass.value("id").toInt());
        dp->setKId(qDoktorPass.value("kid").toInt());
        dp->setSifre(qDoktorPass.value("sifre").toString());

        tblDoktorPass.ramEkle(dp);
    }

    QSqlQuery qYoneticiPass("SELECT * FROM yoneticipass");
    while (qYoneticiPass.next()) {
        auto dp=tblYoneticiPass.olustur();
        dp->setId(qYoneticiPass.value("id").toInt());
        dp->setKId(qYoneticiPass.value("kid").toInt());
        dp->setSifre(qYoneticiPass.value("sifre").toString());

        tblYoneticiPass.ramEkle(dp);
    }
}

DoktorTablosu &VERITABANI::doktorlar() { return tblDoktor; }
HastaTablosu &VERITABANI::hastalar() { return tblHasta; }
ZiyaretTablosu &VERITABANI::ziyaretler() { return tblZiyaret; }
BulguTablosu &VERITABANI::bulgular() { return tblBulgu; }
IstenenTetkikTablosu &VERITABANI::istenentetkikler() { return tblIstenenTetkik; }
ReceteTablosu &VERITABANI::receteler() { return tblRecete; }
TetkikTablosu &VERITABANI::tetkikler() { return tblTetkik; }
IlacTablosu &VERITABANI::ilaclar() { return tblIlac; }
ReceteKalemiTablosu &VERITABANI::recetekalemleri() { return tblReceteKalemi; }
LoginPassTablosu &VERITABANI::doktorsifreleri() { return tblDoktorPass; }
LoginPassTablosu &VERITABANI::yoneticisifreleri() { return tblYoneticiPass; }
DuyuruTablosu &VERITABANI::duyurular() { return tblDuyuru; }

quint32 VERITABANI::aktifkid() const { return _aktifkid; }
void VERITABANI::setAktifkid(quint32 newAktifkid) { _aktifkid = newAktifkid; }
