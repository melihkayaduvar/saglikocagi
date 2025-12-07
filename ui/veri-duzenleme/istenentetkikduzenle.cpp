#include "istenentetkikduzenle.h"
#include "ui/veri-duzenleme/ui_istenentetkikduzenle.h"
#include "../../Veri/veritabani.h"

#include <QMessageBox>

istenentetkikduzenle::istenentetkikduzenle(quint32 &id,QWidget *parent) :
    QDialog(parent), veri_id(id),
    ui(new Ui::istenentetkikduzenle)
{
    ui->setupUi(this);
    loadData();
    connect(ui->cbyenidurum, &QComboBox::currentTextChanged,
            this, &istenentetkikduzenle::onYeniDurumChanged);
    getVeri();
    ui->dtesonuctarihi->setDateTime(QDateTime::currentDateTime());

}

istenentetkikduzenle::~istenentetkikduzenle()
{
    delete ui;
}

void istenentetkikduzenle::loadData()
{
    veri = VERITABANI::vt().istenentetkikler().IdyeGoreAra(veri_id);
    if (!veri) {
        qWarning() << "Veri bulunamadı. ID:" << veri_id;
        return;
    }
    ui->dteistektarihi->setDateTime(veri->istekTarihi());
    ui->dtesonuctarihi->setDateTime(veri->sonucTarihi());
    ui->leistenentetkik->setText(tr("Tetkik ID: %1").arg(veri->tetkikid()));
    TetkikDurumuEnum durum=veri->durum();
    QString durumtext;
    if(durum==TetkikDurumuEnum::CALISILIYOR){
        durumtext="Çalışılıyor";
    }else if(durum==TetkikDurumuEnum::IPTAL_EDILDI){
        durumtext="İptal Edildi";
    }else if(durum==TetkikDurumuEnum::ORNEK_ALINDI){
        durumtext="Örnek Alındı";
    }else if(durum==TetkikDurumuEnum::SONUCLANDI){
        durumtext="Sonuçlandı";
    }else if(durum==TetkikDurumuEnum::ISTENDI){
        durumtext="İstendi";
    }
    ui->lemevcutdrurum->setText(durumtext);
    ui->lesonuc->setText(veri->sonuc());
    ui->teyorum->setText(veri->yorum());
    auto ziyaret = VERITABANI::vt().ziyaretler().IdyeGoreAra(veri->ziyaretid());
    auto hasta = VERITABANI::vt().hastalar().IdyeGoreAra(ziyaret->hastaid());
    ui->leziyaret->setText(tr("Ziyaret ID: %1 Hasta:%2").arg(veri->ziyaretid()).arg(hasta->adi()+" "+hasta->soyadi()));
    ui->cbyenidurum->setCurrentText(durumtext);
}

void istenentetkikduzenle::onYeniDurumChanged(const QString& yeniDurum)
{
    bool isSonuc = (yeniDurum == "Sonuçlandı");

    ui->dtesonuctarihi->setEnabled(isSonuc);
    ui->lesonuc->setEnabled(isSonuc);
    ui->lblsonuc->setEnabled(isSonuc);
    ui->lblsonuctarihi->setEnabled(isSonuc);
}

IstenenTetkikTablosu::VeriPointer istenentetkikduzenle::getVeri() const
{
    if (!veri) {
        qWarning() << "HATA: veri bulunamadı!";
        return nullptr;
    }
    QString yeniDurumStr = ui->cbyenidurum->currentText();
    TetkikDurumuEnum yeniDurum = TetkikDurumuEnum::ISTENDI;

    if (yeniDurumStr == "Çalışılıyor") {
        yeniDurum = TetkikDurumuEnum::CALISILIYOR;
    } else if (yeniDurumStr == "İptal Edildi") {
        yeniDurum = TetkikDurumuEnum::IPTAL_EDILDI;
    } else if (yeniDurumStr == "Örnek Alındı") {
        yeniDurum = TetkikDurumuEnum::ORNEK_ALINDI;
    } else if (yeniDurumStr == "Sonuçlandı") {
        yeniDurum = TetkikDurumuEnum::SONUCLANDI;
    }
    veri->setDurum(yeniDurum);

    if (yeniDurum == TetkikDurumuEnum::SONUCLANDI) {
        veri->setSonuc(ui->lesonuc->text());
        veri->setSonucTarihi(ui->dtesonuctarihi->dateTime());
    }
    veri->setYorum(ui->teyorum->toPlainText());
    return veri;
}

void istenentetkikduzenle::accept()
{
    auto answer = QMessageBox::question(this,
                        tr("Onay"),
                        tr("Düzenlemeyi kaydetmek istiyormusunuz?"));
    if (answer==QMessageBox::Yes){
        QDialog::accept();

    }
}

void istenentetkikduzenle::on_btnKaydet_clicked()
{
    istenentetkikduzenle::accept();
}

