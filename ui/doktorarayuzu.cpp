#include "doktorarayuzu.h"
#include "ui/ui_doktorarayuzu.h"
#include "../Veri/veritabani.h"
#include "veri-giris/ziyaretekle.h"
#include "veri-liste/dkhastaliste.h"
#include "veri-liste/ziyaretliste.h"
#include <QMessageBox>

doktorarayuzu::doktorarayuzu(quint32 dokid,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::doktorarayuzu),
    m_doktorID(dokid)
{
    ui->setupUi(this);
    doktorarayuzu::duyurulariYenile();
}

doktorarayuzu::~doktorarayuzu()
{
    delete ui;
}

void doktorarayuzu::duyurulariYenile()
{
    ui->tbDuyurular->clear();
    auto veri = VERITABANI::vt().duyurular().tumu();

    for(auto it = veri.rbegin(); it != veri.rend(); ++it)
    {
        auto d = *it;
        QString baslik = d->baslik();
        QString icerik = d->metin();
        QString tarih = d->tarih().toString("dd.MM.yyyy");
        QString html = QString(
                           "<div style='border-bottom: 1px solid #ccc; margin-bottom: 10px; padding-bottom: 5px;'>"
                           "<b><font color='#2980b9' size='4'>%1</font></b>" // Başlık
                           "&nbsp;&nbsp;<small style='color:gray;'>(%2)</small><br>" // Tarih
                           "<span style='font-size: 14px;'>%3</span>" // İçerik
                           "</div>"
                           ).arg(baslik, tarih, icerik);
        ui->tbDuyurular->append(html);
    }
    ui->tbDuyurular->moveCursor(QTextCursor::Start);
}

void doktorarayuzu::on_btnHastalar_clicked()
{
    dkHastaListe hastafrm;
    hastafrm.setAttribute(Qt::WA_QuitOnClose, false);
    hastafrm.exec();
}


void doktorarayuzu::on_btnZiyaret_clicked()
{
    ziyaretliste frm(-1,this);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    frm.exec();
}


void doktorarayuzu::on_btnZiyaretOlustur_clicked()
{
    ZiyaretEkle frm(m_doktorID,this);
    auto ziyaret = VERITABANI::vt().ziyaretler().olustur();
    frm.setVeri(ziyaret);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    auto cevap = frm.exec();
    if(cevap==QDialog::Accepted){
        ziyaret=frm.getVeri();
        VERITABANI::vt().ziyaretler().ekle(ziyaret);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 tarihli ziyaret eklendi.").arg(ziyaret->tarihsaat().toString()));
    }
}


void doktorarayuzu::on_actCikis_triggered()
{
    //VERITABANI::vt().kaydet();
    ui->tbDuyurular->clear();
    emit oturumKapatildi();
    this->close();
}
