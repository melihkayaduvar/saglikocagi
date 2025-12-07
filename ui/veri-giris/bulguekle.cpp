#include "bulguekle.h"
#include "ui/veri-giris/ui_bulguekle.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

bulguekle::bulguekle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::bulguekle)
{
    ui->setupUi(this);
    hastaara();
    ziyaretara();
    ui->dteKaydedilmeZamani->setDateTime(QDateTime::currentDateTime());
    connect(ui->HastatableWidget, &QTableWidget::itemSelectionChanged ,this, &bulguekle::Hastatable);
    ui->HastatableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->HastatableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ZiyarettableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ZiyarettableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

bulguekle::~bulguekle()
{
    delete ui;
}

void bulguekle::hastaara()
{
    hastaliste.clear();
    if(ui->leHastaBul->text().trimmed().isEmpty()){
        hastaliste=VERITABANI::vt().hastalar().tumu();
    }else{
        hastaliste=VERITABANI::vt().hastalar().bul([this](HastaTablosu::VeriPointer d){
            return d->adi().toLower().contains(ui->leHastaBul->text().toLower())||
                    d->soyadi().toLower().contains(ui->leHastaBul->text().toLower())||
                    d->tckimlik().contains(ui->leHastaBul->text())||
                    QString::number(d->id()).contains(ui->leHastaBul->text());
        });
    }
    hastatabloguncelle();
}

void bulguekle::hastatabloguncelle()
{
    ui->HastatableWidget->setRowCount(hastaliste.size());
    ui->HastatableWidget->setColumnCount(4);

    QStringList basliklar;
    basliklar<<"ID"<<"ADI"<<"SOYADI"<<"TC KIMLIK";
    ui->HastatableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<hastaliste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(hastaliste[i]->id()));
        ui->HastatableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(hastaliste[i]->adi()));
        ui->HastatableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(hastaliste[i]->soyadi()));
        ui->HastatableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(hastaliste[i]->tckimlik()));
        ui->HastatableWidget->setItem(i,3,hucre3);
    }
}

void bulguekle::ziyaretara()
{
    ziyaretliste.clear();
    int satir=ui->HastatableWidget->currentRow();
    if(satir<0 || !ui->HastatableWidget->item(satir,0)){
        ui->ZiyarettableWidget->clearContents();
        ui->ZiyarettableWidget->setRowCount(0);
        return;
    }
    auto secilenid = ui->HastatableWidget->item(satir,0)->text().toUInt();
    if(!ui->ZiyarettableWidget->isEnabled()){
        ziyaretliste.clear();
    }else if(ui->leZiyaretBul->text().trimmed().isEmpty()){
        ziyaretliste = VERITABANI::vt().ziyaretler().bul([&secilenid](ZiyaretTablosu::VeriPointer d){
            return d->hastaid()==secilenid;
        });
    }else {
        ziyaretliste = VERITABANI::vt().ziyaretler().bul([&secilenid, this](ZiyaretTablosu::VeriPointer d){
            auto doktor = VERITABANI::vt().doktorlar().IdyeGoreAra(d->doktorid());
            return (d->hastaid()==secilenid)&&(
                    d->tarihsaat().toString().toLower().contains(ui->leZiyaretBul->text().toLower())||
                    QString::number(d->id()).contains(ui->leZiyaretBul->text())||
                    doktor->adi().toLower().contains(ui->leZiyaretBul->text().toLower())||
                    doktor->soyadi().toLower().contains(ui->leZiyaretBul->text().toLower()));
        });
    }
    ziyarettabloguncelle();
}
void bulguekle::ziyarettabloguncelle(){
    ui->ZiyarettableWidget->setRowCount(ziyaretliste.size());
    ui->ZiyarettableWidget->setColumnCount(5);

    QStringList basliklar;
    basliklar<<"ID"<<"SİKAYET"<<"TANI"<<"TEDAVİ NOTLARI"<<"DOKTOR";

    ui->ZiyarettableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<ziyaretliste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(ziyaretliste[i]->id()));
        ui->ZiyarettableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(ziyaretliste[i]->sikayet()));
        ui->ZiyarettableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(ziyaretliste[i]->tani()));
        ui->ZiyarettableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(ziyaretliste[i]->tedavinotlari()));
        ui->ZiyarettableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        auto doktor = VERITABANI::vt().doktorlar().IdyeGoreAra(ziyaretliste[i]->doktorid());
        hucre4->setText(tr("%1").arg(doktor->adi()+" "+doktor->soyadi()));
        ui->ZiyarettableWidget->setItem(i,4,hucre4);
    }
}

BulguTablosu::VeriPointer bulguekle::getVeri() const
{
    veri->setAciklama(ui->teAciklama->toPlainText());
    veri->setKaydedilmeZamani(ui->dteKaydedilmeZamani->dateTime());
    BulguTipiEnum tip;
    if(ui->cbTip->currentText()=="Semptom"){
        tip=BulguTipiEnum::SEMPTOM;
    }else if(ui->cbTip->currentText()=="Fiziksel Muayene"){
        tip=BulguTipiEnum::FIZIKSEL_MUAYENE;
    }else if(ui->cbTip->currentText()=="Labaratuvar"){
        tip=BulguTipiEnum::LABARATUVAR;
    }else{
        tip=BulguTipiEnum::GORUNTULEME;
    }
    veri->setTip(tip);
    auto satir= ui->ZiyarettableWidget->currentRow();
    auto id=ui->ZiyarettableWidget->item(satir,0)->text().toUInt();
    veri->setZiyaretId(id);
    return veri;
}

void bulguekle::setVeri(BulguTablosu::VeriPointer newVeri)
{
    veri = newVeri;
    ui->teAciklama->setText(veri->aciklama());
    QString tip;
    if(veri->tip()==BulguTipiEnum::SEMPTOM){
        tip="Semptom";
    }else if(veri->tip()==BulguTipiEnum::FIZIKSEL_MUAYENE){
        tip="Fiziksel Muayene";
    }else if(veri->tip()==BulguTipiEnum::LABARATUVAR){
        tip="Labaratuvar";
    }else{
        tip="Görüntüleme";
    }
    ui->cbTip->setCurrentText(tip);
    ui->dteKaydedilmeZamani->setDateTime(veri->kaydedilmeZamani());
}

void bulguekle::on_leHastaBul_textChanged(const QString &arg1)
{
    hastaara();
}

void bulguekle::Hastatable()
{
    bool secimVarMi = !ui->HastatableWidget->selectedItems().isEmpty();
    ui->ZiyarettableWidget->setEnabled(secimVarMi);
    ui->leZiyaretBul->setEnabled(secimVarMi);
    ui->lblZiyaretBul->setEnabled(secimVarMi);
    if(secimVarMi){
        ziyaretara();
    }
}

void bulguekle::on_leZiyaretBul_textChanged(const QString &arg1)
{
    ziyaretara();
}
void bulguekle::accept(){
    bool secimVarMi=ui->ZiyarettableWidget->selectedItems().isEmpty();
    if(secimVarMi){
        QMessageBox::warning(this,tr("Uyarı"),tr("Ziyaret Seçin!"));
        return;
    }
    secimVarMi=ui->teAciklama->toPlainText().isEmpty();
    if(secimVarMi){
        QMessageBox::warning(this,tr("Uyarı"),tr("Açıklama Giriniz!"));
        return;
    }
    QDialog::accept();
}
void bulguekle::on_btnKaydet_clicked()
{
    accept();
}

