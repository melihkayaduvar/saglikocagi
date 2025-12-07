#include "istenentetkikekle.h"
#include "ui/veri-giris/ui_istenentetkikekle.h"
#include "../../Veri/veritabani.h"
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

#include <QMessageBox>

IstenenTetkikEkle::IstenenTetkikEkle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IstenenTetkikEkle)
{
    ui->setupUi(this);
    tabloguncelle();
    ziyaretcomboboxveri();
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->dteIstekTarihi->setDateTime(QDateTime::currentDateTime());
}

IstenenTetkikEkle::~IstenenTetkikEkle()
{
    delete ui;
}


IstenenTetkikTablosu::VeriPointer IstenenTetkikEkle::getVeri() const
{
    QString ziyaretid =ui->cbZiyaretler->currentText().section("ID: ", 1).section(" ",0,0);
    veri->setZiyaretId(ziyaretid.toUInt());
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            int tetkikId = item->data(Qt::UserRole).toInt();
            veri->setTetkikId(tetkikId);
            break; // sadece bir tanesi seçilebiliyorsa
        }
    }
    veri->setIstekTarihi(ui->dteIstekTarihi->dateTime());
    if(ui->cbTetkikdurumu->currentText()=="İstendi"){
        veri->setDurum(TetkikDurumuEnum::ISTENDI);
    }else if(ui->cbTetkikdurumu->currentText()=="Çalışılıyor"){
        veri->setDurum(TetkikDurumuEnum::CALISILIYOR);
    }else if(ui->cbTetkikdurumu->currentText()=="İptal Edildi"){
        veri->setDurum(TetkikDurumuEnum::IPTAL_EDILDI);
    }else if(ui->cbTetkikdurumu->currentText()=="Örnek Alındı"){
        veri->setDurum(TetkikDurumuEnum::ORNEK_ALINDI);
    }else{
        veri->setDurum(TetkikDurumuEnum::SONUCLANDI);
    }

    return veri;
}

void IstenenTetkikEkle::setVeri(const IstenenTetkikTablosu::VeriPointer &newVeri)
{
    veri = newVeri;
    ui->dteIstekTarihi->setDateTime(veri->istekTarihi());
}

void IstenenTetkikEkle::tabloguncelle()
{
    auto tumtetkikler=VERITABANI::vt().tetkikler().tumu();
    ui->listWidget->clear();
    for (auto i = 0; i < tumtetkikler.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem("ID: "+QString::number(tumtetkikler[i]->id())+" Tetkik: "+tumtetkikler[i]->ad());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::UserRole, QVariant::fromValue(tumtetkikler[i]->id()));
        ui->listWidget->addItem(item);
    }
    connect(ui->listWidget, &QListWidget::itemChanged,
            this, &IstenenTetkikEkle::onItemChanged);
}
void IstenenTetkikEkle::onItemChanged(QListWidgetItem *changedItem)
{
    if (changedItem->checkState() == Qt::Checked) {
        // Diğer tüm öğeleri pasif hale getir
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            QListWidgetItem *item = ui->listWidget->item(i);
            if (item != changedItem && item->checkState() == Qt::Checked) {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void IstenenTetkikEkle::ziyaretcomboboxveri()
{
    auto tumziyaretler=VERITABANI::vt().ziyaretler().tumu();
    ui->cbZiyaretler->clear();

    for (auto &ziyaret : tumziyaretler) {
        QString a;
        a.append("ID: ");
        a.append(QString::number(ziyaret->id())+" , ");
        auto hasta=VERITABANI::vt().hastalar().IdyeGoreAra(ziyaret->hastaid());
        auto doktor=VERITABANI::vt().doktorlar().IdyeGoreAra(ziyaret->doktorid());
        a.append(hasta->adi()+" ");
        a.append(hasta->soyadi()+", ");
        a.append("Doktor:"+doktor->adi()+" "+doktor->soyadi()+", ");
        a.append("Tanı:"+ziyaret->tani()+", ");
        a.append(ziyaret->tarihsaat().toString());

        ui->cbZiyaretler->addItem(a);
    }
}
void IstenenTetkikEkle::accept()
{
    // En az bir tetkik seçili mi kontrol et
    bool seciliVar = false;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            seciliVar = true;
            break;
        }
    }

    if (!seciliVar) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir tetkik seçin!");
        return; // Dialog'u kapatma
    }

    // Eğer seçili varsa normal şekilde dialog'u kapat
    QDialog::accept();
}

void IstenenTetkikEkle::on_btnKaydet_clicked()
{
    IstenenTetkikEkle::accept();
}

