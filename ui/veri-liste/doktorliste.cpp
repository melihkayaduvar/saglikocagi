#include "doktorliste.h"
#include "ui/veri-liste/ui_doktorliste.h"

#include "../../Veri/veritabani.h"
#include "../veri-duzenleme/yenisifrebelirle.h"
#include "../veri-giris/doktorekle.h"

#include <QStringList>
#include <QMessageBox>

DoktorListe::DoktorListe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DoktorListe)
{
    ui->setupUi(this);

    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget,&QTableWidget::itemSelectionChanged,this, &DoktorListe::tablewidget_secimi);
}

DoktorListe::~DoktorListe()
{
    delete ui;
}

void DoktorListe::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste=VERITABANI::vt().doktorlar().bul([](DoktorTablosu::VeriPointer d){
            return true;
        });
    }else {
        liste=VERITABANI::vt().doktorlar().bul([this](DoktorTablosu::VeriPointer d){
            return d->adi().toLower().contains(ui->leAra->text().toLower()) ||
                   d->soyadi().toLower().contains(ui->leAra->text().toLower()) ||
                  d->uzmanlikAlani().toLower().contains(ui->leAra->text().toLower()) ||
                QString::number(d->id()).contains(ui->leAra->text());
        });
    }
    tabloguncelle();
}

void DoktorListe::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

void DoktorListe::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(6);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("ADI");
    basliklar.append("SOYADI");
    basliklar.append("TELEFON");
    basliklar.append("DIPLOMA NO");
    basliklar.append("UZMANLIK ALANI");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(liste[i]->adi());
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(liste[i]->soyadi());
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(liste[i]->telefon());
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(liste[i]->diplomaNo());
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(liste[i]->uzmanlikAlani());
        ui->tableWidget->setItem(i,5,hucre5);
    }
}

void DoktorListe::tablewidget_secimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
    ui->btnYeniSifre->setEnabled(secimVarMi);
}
void DoktorListe::on_btnSil_clicked()
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
            VERITABANI::vt().doktorlar().IdyeGoreSil(silinecekid);
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 ID'li Doktor Silindi").arg(silinecekid));

        }

    }
}

void DoktorListe::on_BtnDoktorOlustur_clicked()
{
    doktorekle doktorekleform;
    auto doktor = VERITABANI::vt().doktorlar().olustur();
    doktorekleform.setAttribute(Qt::WA_QuitOnClose, false);
    doktorekleform.setVeri(doktor);

    auto cevap = doktorekleform.exec();
    if(cevap==QDialog::Accepted){
        doktor = doktorekleform.getVeri();
        if(doktor){
            VERITABANI::vt().doktorlar().ekle(doktor);
            for(auto &i:VERITABANI::vt().doktorlar().tumu()){
                bool varmi=false;
                for(auto &j:VERITABANI::vt().doktorsifreleri().tumu()){
                    if(i->id()==j->kid()){
                        varmi=true;
                        break;
                    }
                }
                if(!varmi){
                    auto yeni=VERITABANI::vt().doktorsifreleri().olustur();
                    yeni->setKId(i->id());
                    yeni->setSifre("-1");
                    VERITABANI::vt().doktorsifreleri().ekle(yeni);
                }
            }

            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 %2 adlı doktor eklendi.").arg(doktor->adi(), doktor->soyadi()));
        }else{
            QMessageBox::warning(this,
                                 tr("Hatalı Bilgi"),
                                 tr("Bu diploma numarası zaten mevcut!"));
        }

    }
    ara();
}


void DoktorListe::on_btnYeniSifre_clicked()
{
    auto selectedRanges = ui->tableWidget->selectedRanges();
    QSet<int> selectedRows;
    for (auto &range : selectedRanges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            selectedRows.insert(row);
        }
    }
    if (selectedRows.size() > 1) {
        QMessageBox::warning(this, tr("Uyarı"), tr("Lütfen sadece bir kişi seçiniz."));
        return;
    }
    int satir = ui->tableWidget->currentRow();
    auto item = ui->tableWidget->item(satir, 0);
    if (!item) return;
    quint32 secilenid = ui->tableWidget->item(satir,0)->text().toUInt();
    yenisifrebelirle sifrefrm(secilenid, this);
    sifrefrm.setAttribute(Qt::WA_QuitOnClose, false);
    sifrefrm.exec();
}

