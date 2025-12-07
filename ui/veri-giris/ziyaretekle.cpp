#include "ziyaretekle.h"
#include "ui/veri-giris/ui_ziyaretekle.h"

#include <QString>

#include "../../Veri/veritabani.h"

ZiyaretEkle::ZiyaretEkle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ZiyaretEkle)
{
    ui->setupUi(this);

    DoktorComboBoxVeri();
    HastaComboBoxVeri();

    ui->dteZiyaret->setDateTime(QDateTime::currentDateTime());
}

ZiyaretEkle::~ZiyaretEkle()
{
    delete ui;
}

ZiyaretTablosu::VeriPointer ZiyaretEkle::getVeri() const
{
    QString docid =ui->doctorComboBox->currentText().section("ID: ", 1).section(" ",0,0);
    QString hastid =ui->hastaComboBox->currentText().section("ID: ", 1).section(" ",0,0);

    veri->setDoktorId(docid.toUInt());
    veri->setHastaId(hastid.toUInt());
    veri->setSikayet(ui->teSikayet->toPlainText());
    veri->setTani(ui->teTani->toPlainText());
    veri->setTedavinotlari(ui->teTedaviNotlari->toPlainText());
    veri->setTarihsaat(ui->dteZiyaret->dateTime());

    return veri;
}

void ZiyaretEkle::setVeri(const ZiyaretTablosu::VeriPointer &newVeri)
{
    veri = newVeri;

    ui->teSikayet->setText(veri->sikayet());
    ui->teTani->setText(veri->tani());
    ui->teTedaviNotlari->setText(veri->tedavinotlari());
    ui->dteZiyaret->setDateTime(veri->tarihsaat());
}

void ZiyaretEkle::DoktorComboBoxVeri()
{
    auto tumdoktorlar=VERITABANI::vt().doktorlar().tumu();
    ui->doctorComboBox->clear();

    for (auto &doctor : tumdoktorlar) {
        QString a;
        a.append("ID: ");
        a.append(QString::number(doctor->id())+" , ");
        a.append("Doktor: "+doctor->adi()+" ");
        a.append(doctor->soyadi()+" , ");
        a.append("Uzm. Alanı: "+doctor->uzmanlikAlani());

        ui->doctorComboBox->addItem(a);
    }
}

void ZiyaretEkle::HastaComboBoxVeri()
{
    auto tumhastalar=VERITABANI::vt().hastalar().tumu();
    ui->hastaComboBox->clear();

    for (auto &hasta : tumhastalar) {
        QString a;
        a.append("ID: ");
        a.append(QString::number(hasta->id())+" , ");
        a.append("Hasta: "+hasta->adi()+" ");
        a.append(hasta->soyadi()+" , ");
        a.append("Tc: "+hasta->tckimlik()+" , ");
        QString hastacinsiyet;
        CinsiyetEnum cinsiyet = hasta->cinsiyet();
        switch (cinsiyet) {
        case CinsiyetEnum::ERKEK:
            hastacinsiyet="Erkek";
            break;
        case CinsiyetEnum::KADIN:
            hastacinsiyet="Kadın";
            break;
        case CinsiyetEnum::BELIRTILMEMIS:
            break;
        }
        a.append(hastacinsiyet);

        ui->hastaComboBox->addItem(a);
    }
}
