#include "hastabilgi.h"
#include "ui/veri-liste/ui_hastabilgi.h"
#include "../veri-duzenleme/hastaduzenle.h"
#include "../../Veri/veritabani.h"
#include <QMessageBox>


HastaBilgi::HastaBilgi(quint32 id, QWidget *parent)
    : QDialog(parent), veri_id(id)
    , ui(new Ui::HastaBilgi)
{
    ui->setupUi(this);
    loadData();
}

HastaBilgi::~HastaBilgi()
{
    delete ui;
}

void HastaBilgi::loadData()
{
    auto hasta = VERITABANI::vt().hastalar().IdyeGoreAra(veri_id);
    ui->leAdi->setText(hasta->adi());
    ui->leSoyadi->setText(hasta->soyadi());
    ui->leKimlikNo->setText(hasta->tckimlik());
    ui->leTel->setText(hasta->telefon());
    CinsiyetEnum cinsiyet = hasta->cinsiyet();
    switch (cinsiyet) {
    case CinsiyetEnum::ERKEK:
        ui->leCinsiyet->setText("Erkek");
        break;
    case CinsiyetEnum::KADIN:
        ui->leCinsiyet->setText("Kadın");
        break;
    case CinsiyetEnum::BELIRTILMEMIS:
        break;
    }
    ui->leKanGrubu->setText(hasta->kanGrubu());
    ui->teAdres->setText(hasta->adres());
    ui->lwAlerjiler->clear();
    for (QString &item : hasta->alerjiler()) {
        if (!item.trimmed().isEmpty()) {
            ui->lwAlerjiler->addItem(item);
        }
    }
    ui->lwKronikHastaliklar->clear();
    for (QString &item : hasta->kronikHastaliklar()) {
        if (!item.trimmed().isEmpty()) {
            ui->lwKronikHastaliklar->addItem(item);
        }
    }
}

void HastaBilgi::on_btnDuzenle_clicked()
{
    hastaduzenle frm(veri_id, this);
    if(frm.exec() == QDialog::Accepted){
        auto guncelVeri = frm.getVeri();
        if(!guncelVeri) return;
        VERITABANI::vt().hastalar().duzenle(veri_id,[guncelVeri](auto& veri){
            auto asilID=veri->id();
            *veri=*guncelVeri;
            veri->setId(asilID);
        });
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 idli istenen hasta güncellendi.").arg(veri_id));

        loadData();
    }
}
