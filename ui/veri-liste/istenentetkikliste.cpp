#include "istenentetkikliste.h"
#include "ui/veri-liste/ui_istenentetkikliste.h"
#include "../veri-duzenleme/istenentetkikduzenle.h"
#include "../../Veri/veritabani.h"
#include <QMessageBox>

istenentetkikliste::istenentetkikliste(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::istenentetkikliste)
{
    ui->setupUi(this);
    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->btnDuzenle, &QPushButton::clicked, this, &istenentetkikliste::duzenleTiklandi);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,this, &istenentetkikliste::tablewidget_silmesecimi);
}

istenentetkikliste::~istenentetkikliste()
{
    delete ui;
}
QString TetkikDurumToString(TetkikDurumuEnum durum) {
    switch (durum) {
    case TetkikDurumuEnum::ISTENDI: return "istendi";
    case TetkikDurumuEnum::CALISILIYOR: return "ÇAŞILIŞILIYOR";
    case TetkikDurumuEnum::IPTAL_EDILDI: return "İPTAL EDİLDİ";
    case TetkikDurumuEnum::ORNEK_ALINDI: return "ÖRNEK ALINDI";
    case TetkikDurumuEnum::SONUCLANDI: return "SONUÇLANDI";
    default: return "BİLİNMİYOR";
    }
}
void istenentetkikliste::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().istenentetkikler().tumu();
    }else {
        liste = VERITABANI::vt().istenentetkikler().bul([this](IstenenTetkikTablosu::VeriPointer d){
            auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(d->ziyaretid());
            auto id=ziyaret->hastaid();
            auto hasta= VERITABANI::vt().hastalar().bul([&id](HastaTablosu::VeriPointer d){
                return  d->id()==id;
            });
            auto tetkik = VERITABANI::vt().tetkikler().IdyeGoreAra(d->tetkikid());
            return QString::number(d->id()).contains(ui->leAra->text())||
                QString::number(d->ziyaretid()).contains(ui->leAra->text())||
                hasta[0]->adi().toLower().contains(ui->leAra->text().toLower())||
                hasta[0]->soyadi().toLower().contains(ui->leAra->text().toLower())||
                tetkik->ad().toLower().contains(ui->leAra->text().toLower())||
                ("ID:"+QString::number(tetkik->id())).contains(ui->leAra->text())||
                TetkikDurumToString(d->durum()).toLower().contains(ui->leAra->text());
        });
    }
    tabloguncelle();
}

void istenentetkikliste::duzenleTiklandi()
{
    auto secili = ui->tableWidget->selectedItems();
    if (secili.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir satır seçiniz.");
        return;
    }

    int row = secili.first()->row();
    quint32 id = ui->tableWidget->item(row, 0)->text().toUInt();

    istenentetkikduzenle frm(id, this);
    if(frm.exec() == QDialog::Accepted){
        auto guncelVeri = frm.getVeri();
        VERITABANI::vt().istenentetkikler().duzenle(id,[guncelVeri](auto& veri){
            *veri=*guncelVeri;
        });
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 idli istenen tetkik güncellendi.").arg(id));

        tabloguncelle();
    }
}



void istenentetkikliste::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(9);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("ISTEK TARIHI");
    basliklar.append("SONUC TARIHI");
    basliklar.append("SONUC");
    basliklar.append("YORUM");
    basliklar.append("DURUM");
    basliklar.append("HASTA");
    basliklar.append("ZIYARET ID");
    basliklar.append("TETKIK");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->istekTarihi().toString()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->sonucTarihi().toString()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->sonuc()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(liste[i]->yorum()));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        TetkikDurumuEnum durum = liste[i]->durum();
        switch (durum) {
        case TetkikDurumuEnum::ISTENDI:
            hucre5->setText("İSTENDİ");
            break;
        case TetkikDurumuEnum::CALISILIYOR:
            hucre5->setText("ÇALIŞILIYOR");
            break;
        case TetkikDurumuEnum::IPTAL_EDILDI:
            hucre5->setText("İPTAL EDİLDİ");
            break;
        case TetkikDurumuEnum::ORNEK_ALINDI:
            hucre5->setText("ÖRNEK ALINDI");
            break;
        case TetkikDurumuEnum::SONUCLANDI:
            hucre5->setText("SONUÇLANDI");
            break;
        }
        ui->tableWidget->setItem(i,5,hucre5);
        QTableWidgetItem *hucre6 =new QTableWidgetItem;
        auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(liste[i]->ziyaretid());
        auto id=ziyaret->hastaid();
        auto hasta=VERITABANI::vt().hastalar().bul([&id](HastaTablosu::VeriPointer d){
            return d->id()==id;
        });
        hucre6->setText(tr("%1").arg(hasta[0]->adi()+" "+hasta[0]->soyadi()));
        ui->tableWidget->setItem(i,6,hucre6);
        QTableWidgetItem *hucre7=new QTableWidgetItem;
        hucre7->setText(tr("%1").arg(liste[i]->ziyaretid()));
        ui->tableWidget->setItem(i,7,hucre7);
        QTableWidgetItem *hucre8 =new QTableWidgetItem;
        auto tetkik = VERITABANI::vt().tetkikler().IdyeGoreAra(liste[i]->tetkikid());
        hucre8->setText(tr("%1").arg("ID:"+QString::number(tetkik->id())+" "+tetkik->ad()));
        ui->tableWidget->setItem(i,8,hucre8);
    }
}
void istenentetkikliste::tablewidget_silmesecimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
    ui->btnDuzenle->setEnabled(secimVarMi);

}
void istenentetkikliste::on_btnSil_clicked()
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
            VERITABANI::vt().istenentetkikler().IdyeGoreSil(silinecekid);
        }
    }
}


void istenentetkikliste::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

