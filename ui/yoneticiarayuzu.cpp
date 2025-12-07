#include "yoneticiarayuzu.h"
#include "ui_yoneticiarayuzu.h"
#include "veri-liste/doktorliste.h"
#include "veri-liste/hastaliste.h"
#include "veri-liste/ilacliste.h"
#include "veri-liste/tetkikliste.h"
#include "../Veri/veritabani.h"
#include <qmessagebox.h>
#include <qvalidator.h>

yoneticiarayuzu::yoneticiarayuzu(quint32 yonid,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::yoneticiarayuzu),
    m_yoneticiID(yonid)
{
    ui->setupUi(this);
    yenile();
    yoneticiarayuzu::duyurulariYenile();
    ui->leID->setValidator(new QIntValidator(0, 999999, this));

}
yoneticiarayuzu::~yoneticiarayuzu()
{
    delete ui;
}

void yoneticiarayuzu::yenile()
{
    ui->leDoktorSayisi->setText(QString::number(VERITABANI::vt().doktorlar().boyut()));
    ui->leHastaSayisi->setText(QString::number(VERITABANI::vt().hastalar().boyut()));
    ui->leToplamZiyaretSayisi->setText(QString::number(VERITABANI::vt().ziyaretler().boyut()));
    ui->leYazilanReceteSayisi->setText(QString::number(VERITABANI::vt().receteler().boyut()));
    ui->leEnCokYazilanIlac->setText(enCokYazilanIlac());
}



QString yoneticiarayuzu::enCokYazilanIlac()
{
    QMap<QString, int> ilacSayaci;

    for (auto& recete : VERITABANI::vt().recetekalemleri().tumu()) {
        auto id=recete->ilacid();try {
            auto ilac = VERITABANI::vt().ilaclar().IdyeGoreAra(id);
            ilacSayaci[ilac->ad()]++;
        } catch (...) {
            continue;
        }
    }

    QString enCokIlac;
    int maxSayi = 0;

    for (auto it = ilacSayaci.constBegin(); it != ilacSayaci.constEnd(); ++it) {
        if (it.value() > maxSayi) {
            maxSayi = it.value();
            enCokIlac = it.key();
        }
    }

    return enCokIlac;
}

void yoneticiarayuzu::on_BtnDoktor_clicked()
{
    DoktorListe doktorfrm;
    doktorfrm.exec();
    yenile();
}


void yoneticiarayuzu::on_BtnHasta_clicked()
{
    HastaListe hastafrm;
    hastafrm.exec();
    yenile();
}


void yoneticiarayuzu::on_BtnIlac_clicked()
{
    IlacListe ilacfrm;
    ilacfrm.exec();
    yenile();
}


void yoneticiarayuzu::on_BtnTetkik_clicked()
{
    TetkikListe tetkikfrm;
    tetkikfrm.exec();
    yenile();
}


void yoneticiarayuzu::on_BtnDuyuru_clicked()
{
    if(ui->leDuyuruBaslik->text().isEmpty()||ui->teDuyuruMetin->toPlainText().isEmpty()){
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Başlık veya Metin Boş Olamaz!"));
        return;
    }
    auto x = VERITABANI::vt().duyurular().olustur();
    x->setBaslik(ui->leDuyuruBaslik->text());
    x->setMetin(ui->teDuyuruMetin->toPlainText());
    x->setTarih(QDate::currentDate());
    VERITABANI::vt().duyurular().ekle(x);
    QMessageBox::information(this,
                    tr("%1").arg("Başarılı"),
                    tr("%1").arg("'"+ui->leDuyuruBaslik->text()+"'"+"başlıklı duyuru başarı ile eklendi."));
    yoneticiarayuzu::duyurulariYenile();
}
void yoneticiarayuzu::duyurulariYenile()
{
    ui->tbDuyurular->clear();
    auto veri = VERITABANI::vt().duyurular().tumu();

    for(auto it = veri.rbegin(); it != veri.rend(); ++it)
    {
        auto d = *it;
        quint32 id = d->id();
        QString baslik = d->baslik();
        QString icerik = d->metin();
        QString tarih = d->tarih().toString("dd.MM.yyyy");
        QString html = QString(
                           "<div style='border-bottom: 1px solid #ccc; margin-bottom: 10px; padding-bottom: 5px;'>"
                           "<b><font color='#2980b9' size='4'>%1</font></b>"
                           "&nbsp;&nbsp;<small style='color:gray;'>Islem Tarihi: %2 | <b>ID: #%4</b></small><br>"
                           "<span style='font-size: 14px; margin-top: 5px; display:block;'>%3</span>"
                           "</div>"
                           ).arg(baslik)
                           .arg(tarih)
                           .arg(icerik)
                           .arg(id);
        ui->tbDuyurular->append(html);
    }
    ui->tbDuyurular->moveCursor(QTextCursor::Start);
}

void yoneticiarayuzu::on_btnTumuSil_clicked()
{
    auto silinecekler = VERITABANI::vt().duyurular().tumu();
    if(!silinecekler.isEmpty()){
        auto cevap=QMessageBox::question(this,
                                           tr("Onay"),
                                           tr("Tüm duyurular silinecektir. Bu işlemi onaylıyormusunuz?"));
        if(cevap==QMessageBox::Yes){
            auto boyut=VERITABANI::vt().duyurular().boyut();
            for(auto &d : silinecekler) {
                VERITABANI::vt().duyurular().IdyeGoreSil(d->id());
            }
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 Adet duyuru silindi!").arg(boyut));
        }
        duyurulariYenile();
    }else{
        QMessageBox::warning(this,
                            tr("Uyarı"),
                            tr("Silinecek duyuru yok!"));
    }
}


void yoneticiarayuzu::on_btnDuyuruSil_clicked()
{
    auto silinecekid=ui->leID->text().toUInt();
    auto sorgu = VERITABANI::vt().duyurular().bul([&silinecekid](DuyuruTablosu::VeriPointer d){
        return d->id()==silinecekid;
    });
    if(sorgu.isEmpty()){
        QMessageBox::warning(this,
                             tr("Uyarı!"),
                             tr("%1 ID'li ait duyuru yok").arg(silinecekid));
    }else{
        auto cevap=QMessageBox::question(this,
                                           tr("Onay!"),
                                           tr("%1 ID'li duyuruyu silmek istediğinize eminmisiniz?").arg(silinecekid));
        if(cevap==QMessageBox::Yes){
            VERITABANI::vt().duyurular().IdyeGoreSil(silinecekid);
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 ID'li duyuru silindi").arg(silinecekid));
        }
        duyurulariYenile();
    }

}
void yoneticiarayuzu::on_leID_textChanged(const QString &arg1)
{
    ui->btnDuyuruSil->setEnabled(!arg1.isEmpty());
}

void yoneticiarayuzu::on_actExit_triggered()
{
    //VERITABANI::vt().kaydet();
    ui->tbDuyurular->clear();
    emit oturumKapatildi();
    this->close();
}

