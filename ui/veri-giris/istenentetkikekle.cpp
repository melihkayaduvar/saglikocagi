#include "istenentetkikekle.h"
#include "ui/veri-giris/ui_istenentetkikekle.h"
#include "../../Veri/veritabani.h"
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

#include <QMessageBox>

IstenenTetkikEkle::IstenenTetkikEkle(quint32 kid,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IstenenTetkikEkle),
    m_kid(kid)
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
    quint32 ziyaretid =m_kid;
    veri->setZiyaretId(ziyaretid);
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            int tetkikId = item->data(Qt::UserRole).toInt();
            veri->setTetkikId(tetkikId);
            break;
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
    auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(m_kid);
    ui->cbZiyaretler->clear();

    QString a;
    a.append("ID: ");
    a.append(QString::number(ziyaret->id())+" , ");
    auto hasta=VERITABANI::vt().hastalar().IdyeGoreAra(ziyaret->hastaid());
    auto doktorlar = VERITABANI::vt().doktorlar().bul([&](DoktorTablosu::VeriPointer d){
        return d->id()==ziyaret->doktorid();
    });
    a.append(hasta->adi()+" ");
    a.append(hasta->soyadi()+", ");
    if (!doktorlar.isEmpty()) {
        auto doktor = doktorlar[0];
        a.append("Doktor: "+doktor->adi()+" "+doktor->soyadi()+", ");
    } else {
        a.append("Doktor: Bilinmeyen Doktor, ");
    }
    a.append("Tanı:"+ziyaret->tani()+", ");
    a.append(ziyaret->tarihsaat().toString());

    ui->cbZiyaretler->addItem(a);
}
void IstenenTetkikEkle::accept()
{
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
        return;
    }
    QDialog::accept();
}

void IstenenTetkikEkle::on_btnKaydet_clicked()
{
    IstenenTetkikEkle::accept();
}

