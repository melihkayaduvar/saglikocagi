#include "hastaekle.h"
#include "ui/veri-giris/ui_hastaekle.h"
#include "../../Veri/veritabani.h"

HastaEkle::HastaEkle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HastaEkle)
{
    ui->setupUi(this);
    ui->lwAlerjiler->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lwKronikHastaliklar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->lwAlerjiler, &QListWidget::itemSelectionChanged,
            this, &HastaEkle::lwAlerjiler_silmesecimi);
    connect(ui->lwKronikHastaliklar, &QListWidget::itemSelectionChanged,
            this, &HastaEkle::lwKronikHastaliklar_silmesecimi);

}

HastaEkle::~HastaEkle()
{
    delete ui;
}

HastaTablosu::VeriPointer HastaEkle::getVeri() const
{
    veri->setadi(ui->leAdi->text());
    veri->setsoyadi(ui->leSoyadi->text());
    QString masked = ui->leTel->text();
    QString unmasked = masked;
    unmasked.remove(QRegularExpression("[^0-9]"));
    unmasked="0"+unmasked;
    veri->settelefon(unmasked);
    auto x= ui->leKimlikNo->text();
    auto sorgu=VERITABANI::vt().hastalar().bul([&x](HastaTablosu::VeriPointer d){
        return d->tckimlik()==x;
    });
    if(!sorgu.isEmpty()){
        return nullptr;
    }
    veri->setTckimlik(ui->leKimlikNo->text());

    veri->setAdres(ui->teAdres->toPlainText());

    QStringList liste;
    for (auto i=0;i<ui->lwAlerjiler->count();++i){
        liste << ui->lwAlerjiler->item(i)->text();
    }
    veri->setAlerjiler(liste);
    QStringList liste2;
    for (auto i=0;i<ui->lwKronikHastaliklar->count();++i){
        liste2 << ui->lwKronikHastaliklar->item(i)->text();
    }
    veri->setKronikHastaliklar(liste2);

    veri->setDogumTarihi(ui->deDogumTarihi->date());

    veri->setKanGrubu(ui->cbKanGrubu->currentText());

    QString secim=ui->cbCinsiyet->currentText();
    CinsiyetEnum cinsiyet;
    if(secim=="ERKEK")
        cinsiyet= CinsiyetEnum::ERKEK;
    else if(secim=="KADIN")
        cinsiyet= CinsiyetEnum::KADIN;
    else
        cinsiyet=CinsiyetEnum::BELIRTILMEMIS;

    veri->setCinsiyet(cinsiyet);

    return veri;

}

void HastaEkle::setVeri(const HastaTablosu::VeriPointer &newVeri)
{
    veri = newVeri;

    ui->leAdi->setText(veri->adi());
    ui->leSoyadi->setText(veri->soyadi());
    QString temiz=veri->telefon();
    if(temiz.length()==11){
        if(temiz.startsWith("0")){
            temiz.remove(0,1);
        }
        QString masked = QString("(%1) %2 %3 %4")
                             .arg(temiz.mid(0, 3), temiz.mid(3, 3), temiz.mid(6, 2), temiz.mid(8, 2));  // Son 2 rakam

        ui->leTel->setText(masked);
    }else{
        ui->leTel->setText("");
    }
    ui->leTel->setText(veri->telefon());
    ui->leKimlikNo->setText(veri->tckimlik());

    ui->teAdres->setText(veri->adres());


    ui->lwAlerjiler->clear();
    for (QString &item : veri->alerjiler()) {
        if (!item.trimmed().isEmpty()) {
            ui->lwAlerjiler->addItem(item);
        }
    }
    ui->lwKronikHastaliklar->clear();
    for (QString &item : veri->kronikHastaliklar()) {
        if (!item.trimmed().isEmpty()) {
            ui->lwKronikHastaliklar->addItem(item);
        }
    }

    ui->deDogumTarihi->setDate(veri->dogumTarihi());

    ui->cbKanGrubu->setCurrentText(veri->kanGrubu());
    CinsiyetEnum cinsiyet = veri->cinsiyet();
    switch (cinsiyet) {
    case CinsiyetEnum::ERKEK:
        ui->cbCinsiyet->setCurrentText("Erkek");
        break;
    case CinsiyetEnum::KADIN:
        ui->cbCinsiyet->setCurrentText("Kadın");
        break;
    case CinsiyetEnum::BELIRTILMEMIS:
        break;
    }
}

void HastaEkle::on_btnAlerjilerEkle_clicked()
{
    QString girilenVeri = ui->leAlerjiler->text().trimmed();

    if (!girilenVeri.isEmpty()) {
        ui->lwAlerjiler->addItem(girilenVeri);
        ui->leAlerjiler->clear();
    }
}


void HastaEkle::on_btnKronikHastaliklarEkle_clicked()
{
    QString girilenVeri = ui->leKronikHastaliklar->text().trimmed();

    if (!girilenVeri.isEmpty()) {
        ui->lwKronikHastaliklar->addItem(girilenVeri);
        ui->leKronikHastaliklar->clear();
    }
}

void HastaEkle::on_btnAlerjilerSil_clicked()
{
    QListWidgetItem *item = ui->lwAlerjiler->currentItem();
    if (item) {
        // UI'dan sil
        delete item;
    }
}
void HastaEkle::lwAlerjiler_silmesecimi()
{
    bool secimVarMi = !ui->lwAlerjiler->selectedItems().isEmpty();
    ui->btnAlerjilerSil->setEnabled(secimVarMi);
}


void HastaEkle::on_btnKronikHastaliklarSil_clicked()
{
    QListWidgetItem *item = ui->lwKronikHastaliklar->currentItem();
    if (item) {
        // UI'dan sil
        delete item;
    }
}
void HastaEkle::lwKronikHastaliklar_silmesecimi()
{
    bool secimVarMi = !ui->lwKronikHastaliklar->selectedItems().isEmpty();
    ui->btnKronikHastaliklarSil->setEnabled(secimVarMi);
}
