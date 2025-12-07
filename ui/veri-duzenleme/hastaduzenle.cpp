#include "hastaduzenle.h"
#include "ui/veri-duzenleme/ui_hastaduzenle.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

hastaduzenle::hastaduzenle(quint32 id,QWidget *parent)
    : QDialog(parent), veri_id(id)
    , ui(new Ui::hastaduzenle)
{
    ui->setupUi(this);
    loadData();
    ui->lwAlerjiler->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lwKronikHastaliklar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->lwAlerjiler, &QListWidget::itemSelectionChanged,
            this, &hastaduzenle::lwAlerjiler_silmesecimi);
    connect(ui->lwKronikHastaliklar, &QListWidget::itemSelectionChanged,
            this, &hastaduzenle::lwKronikHastaliklar_silmesecimi);

}

hastaduzenle::~hastaduzenle()
{
    delete ui;
}

void hastaduzenle::loadData()
{
    auto asilveri = VERITABANI::vt().hastalar().IdyeGoreAra(veri_id);
    if (!asilveri) {
        qWarning() << "Veri bulunamadı. ID:" << veri_id;
        return;
    }
    veri = VERITABANI::vt().hastalar().olustur();
    *veri=*asilveri;
    ui->leAdi->setText(veri->adi());
    ui->leSoyadi->setText(veri->soyadi());
    CinsiyetEnum cinsiyet=veri->cinsiyet();
    if(cinsiyet==CinsiyetEnum::ERKEK){
        ui->cbCinsiyet->setCurrentText("ERKEK");
    }else if(cinsiyet==CinsiyetEnum::KADIN){
        ui->cbCinsiyet->setCurrentText("KADIN");
    }
    ui->leKimlikNo->setText(veri->tckimlik());
    ui->deDogumTarihi->setDate(veri->dogumTarihi());
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
    ui->teAdres->setText(veri->adres());
    ui->lwAlerjiler->addItems(veri->alerjiler());
    ui->lwKronikHastaliklar->addItems(veri->kronikHastaliklar());
    ui->cbKanGrubu->setCurrentText(veri->kanGrubu());

    ui->leAdi->setEnabled(false);
    ui->leSoyadi->setEnabled(false);
    ui->cbCinsiyet->setEnabled(false);
    ui->leKimlikNo->setEnabled(false);
    ui->deDogumTarihi->setEnabled(false);
    ui->cbKanGrubu->setEnabled(false);
}

HastaTablosu::VeriPointer hastaduzenle::getVeri() const
{
    if (!veri) {
        qWarning() << "HATA: veri bulunamadı!";
        return nullptr;
    }
    QString masked = ui->leTel->text();
    QString unmasked = masked;
    unmasked.remove(QRegularExpression("[^0-9]"));
    unmasked="0"+unmasked;
    veri->settelefon(unmasked);
    veri->setAdres(ui->teAdres->toPlainText());
    QStringList alerjiler,kronikhastaliklar;
    for(auto i=0;i<ui->lwAlerjiler->count();++i){
        alerjiler.append(ui->lwAlerjiler->item(i)->text());
    }
    for(auto i=0;i<ui->lwKronikHastaliklar->count();++i){
        kronikhastaliklar.append(ui->lwKronikHastaliklar->item(i)->text());
    }
    veri->setAlerjiler(alerjiler);
    veri->setKronikHastaliklar(kronikhastaliklar);
    return veri;
}
void hastaduzenle::on_btnAlerjilerEkle_clicked()
{
    QString girilenVeri = ui->leAlerjiler->text().trimmed();

    if (!girilenVeri.isEmpty()) {
        ui->lwAlerjiler->addItem(girilenVeri);
        ui->leAlerjiler->clear();
    }
}


void hastaduzenle::on_btnKronikHastaliklarEkle_clicked()
{
    QString girilenVeri = ui->leKronikHastaliklar->text().trimmed();

    if (!girilenVeri.isEmpty()) {
        ui->lwKronikHastaliklar->addItem(girilenVeri);
        ui->leKronikHastaliklar->clear();
    }
}

void hastaduzenle::on_btnAlerjilerSil_clicked()
{
    QListWidgetItem *item = ui->lwAlerjiler->currentItem();
    if (item) {
        // UI'dan sil
        delete item;
    }
}
void hastaduzenle::lwAlerjiler_silmesecimi()
{
    bool secimVarMi = !ui->lwAlerjiler->selectedItems().isEmpty();
    ui->btnAlerjilerSil->setEnabled(secimVarMi);
}


void hastaduzenle::on_btnKronikHastaliklarSil_clicked()
{
    QListWidgetItem *item = ui->lwKronikHastaliklar->currentItem();
    if (item) {
        // UI'dan sil
        delete item;
    }
}
void hastaduzenle::lwKronikHastaliklar_silmesecimi()
{
    bool secimVarMi = !ui->lwKronikHastaliklar->selectedItems().isEmpty();
    ui->btnKronikHastaliklarSil->setEnabled(secimVarMi);
}
void hastaduzenle::accept()
{
    auto answer = QMessageBox::question(this,
                                        tr("Onay"),
                                        tr("Düzenlemeyi kaydetmek istiyormusunuz?"));
    if (answer==QMessageBox::Yes){
        QDialog::accept();

    }
}
void hastaduzenle::on_btnKaydet_clicked()
{
    accept();
}

