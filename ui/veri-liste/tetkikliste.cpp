#include "tetkikliste.h"
#include "ui/veri-liste/ui_tetkikliste.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>
#include <qtablewidget.h>
#include "../veri-duzenleme/tetkikduzenle.h"
#include "../veri-giris/tetkikekle.h"

TetkikListe::TetkikListe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TetkikListe)
{
    ui->setupUi(this);

    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged ,this, &TetkikListe::tablewidget_silmesecimi);
    connect(ui->btnDuzenle, &QPushButton::clicked,this,&TetkikListe::duzenleTiklandi);
}

TetkikListe::~TetkikListe()
{
    delete ui;
}

void TetkikListe::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().tetkikler().tumu();
    }else {
        liste = VERITABANI::vt().tetkikler().bul([this](TetkikTablosu::VeriPointer d){
            return d->ad().toLower().contains(ui->leAra->text().toLower())||
                QString::number(d->id()).contains(ui->leAra->text());
        });
    }
    tabloguncelle();
}
void TetkikListe::duzenleTiklandi()
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
    auto secili = ui->tableWidget->selectedItems();

    int row = secili.first()->row();
    quint32 id = ui->tableWidget->item(row, 0)->text().toUInt();

    tetkikduzenle frm(id, this);
    if(frm.exec() == QDialog::Accepted){
        auto guncelVeri = frm.getVeri();
        VERITABANI::vt().tetkikler().duzenle(id,[guncelVeri](auto& veri){
            *veri=*guncelVeri;
        });
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 idli istenen tetkik güncellendi.").arg(id));
        tabloguncelle();
    }
}
void TetkikListe::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(5);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("TETKIK ADI");
    basliklar.append("ACIKLAMA");
    basliklar.append("NORMAL DEGERLER ERKEK");
    basliklar.append("NORMAL DEGERLER KADIN");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->ad()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->aciklama()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->normalDegerlerErkek()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(liste[i]->normalDegerlerKadın()));
        ui->tableWidget->setItem(i,4,hucre4);
    }
}
void TetkikListe::tablewidget_silmesecimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
    ui->btnDuzenle->setEnabled(secimVarMi);

}
void TetkikListe::on_btnSil_clicked()
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
            VERITABANI::vt().tetkikler().IdyeGoreSil(silinecekid);
        }
    }
}

void TetkikListe::on_leAra_textChanged(const QString &arg1)
{
    ara();
}


void TetkikListe::on_btnEkle_clicked()
{
    tetkikekle tetkikfrm;
    auto tetkik = VERITABANI::vt().tetkikler().olustur();
    tetkikfrm.setVeri(tetkik);

    auto cevap=tetkikfrm.exec();
    if(cevap==QDialog::Accepted){
        tetkik=tetkikfrm.getVeri();
        if(tetkik){
            VERITABANI::vt().tetkikler().ekle(tetkik);
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 adlı tetkik eklendi.").arg(tetkik->ad()));
        }else{}
    }

    ara();
}

