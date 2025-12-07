#include "bulguliste.h"
#include "ui/veri-liste/ui_bulguliste.h"

#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

bulguliste::bulguliste(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::bulguliste)
{
    ui->setupUi(this);
    ara();
    connect(ui->tableWidget,&QTableWidget::itemSelectionChanged,this,&bulguliste::silmesecimi);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

bulguliste::~bulguliste()
{
    delete ui;
}
QString BulguTipiToString(BulguTipiEnum durum) {
    switch (durum) {
    case BulguTipiEnum::SEMPTOM: return "semptom";
    case BulguTipiEnum::FIZIKSEL_MUAYENE: return "fiziksel muayene";
    case BulguTipiEnum::GORUNTULEME: return "görüntüleme";
    case BulguTipiEnum::LABARATUVAR: return "labaratuvar";
    default: return "BİLİNMİYOR";
    }
}
void bulguliste::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().bulgular().tumu();
    }else {
        liste = VERITABANI::vt().bulgular().bul([this](BulguTablosu::VeriPointer d){
            auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(d->ziyaretid());
            auto id=ziyaret->hastaid();
            auto hasta= VERITABANI::vt().hastalar().bul([&id](HastaTablosu::VeriPointer d){
                return  d->id()==id;
            });
            return QString::number(d->id()).contains(ui->leAra->text())||
                   hasta[0]->adi().toLower().contains(ui->leAra->text().toLower())||
                   hasta[0]->soyadi().toLower().contains(ui->leAra->text().toLower())||
                   BulguTipiToString(d->tip()).toLower().contains(ui->leAra->text().toLower());
        });
    }
    tabloguncelle();
}

void bulguliste::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(6);
    QStringList basliklar;
    basliklar<<"ID"<<"TİPİ"<<"AÇIKLAMA"<<"KAYDEDİLME ZAMANI"<<"HASTA"<<"ZİYARET ID";
    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        QString tip;
        if(liste[i]->tip()==BulguTipiEnum::SEMPTOM){
            tip="Semptom";
        }else if(liste[i]->tip()==BulguTipiEnum::FIZIKSEL_MUAYENE){
            tip="Fiziksel Muayene";
        }else if(liste[i]->tip()==BulguTipiEnum::LABARATUVAR){
            tip="Labaratuvar";
        }else{
            tip="Görüntüleme";
        }
        hucre1->setText(tr("%1").arg(tip));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->aciklama()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->kaydedilmeZamani().toString()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4 =new QTableWidgetItem;
        auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(liste[i]->ziyaretid());
        auto hastaid=ziyaret->hastaid();
        auto hasta=VERITABANI::vt().hastalar().bul([&hastaid](HastaTablosu::VeriPointer d){
            return d->id()==hastaid;
        });
        hucre4->setText(tr("%1").arg(hasta[0]->adi()+" "+hasta[0]->soyadi()));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(tr("%1").arg(liste[i]->ziyaretid()));
        ui->tableWidget->setItem(i,5,hucre5);
    }
}

void bulguliste::silmesecimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
}

void bulguliste::on_btnSil_clicked()
{
    auto selectedRanges = ui->tableWidget->selectedRanges();
    QSet<int> selectedRows;
    for (auto &range : selectedRanges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            selectedRows.insert(row);
        }
    }
    if (selectedRows.size() > 1) {
        QMessageBox::warning(this, tr("Uyarı"), tr("Lütfen sadece bir satır seçiniz."));
        return;
    }
    auto x = QMessageBox::question(this,tr("Onay"),tr("Silme İşlemini Onaylıyormusunuz"));
    if(x==QMessageBox::Yes){
        int satir = ui->tableWidget->currentRow();
        if (satir>=0) {
            auto silinecekid = ui->tableWidget->item(satir,0)->text().toUInt();
            ui->tableWidget->removeRow(satir);
            VERITABANI::vt().bulgular().IdyeGoreSil(silinecekid);
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 ID'li Bulgu Silindi").arg(silinecekid));

        }

    }
}


void bulguliste::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

