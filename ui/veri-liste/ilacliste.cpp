#include "ilacliste.h"
#include "ui_ilacliste.h"

#include "../../Veri/veritabani.h"
#include "../veri-bilgi/ilacbilgi.h"
#include "../veri-giris/ilacekle.h"
#include <qmessagebox.h>

IlacListe::IlacListe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IlacListe)
{
    ui->setupUi(this);
    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged ,this, &IlacListe::tablewidget_silmesecimi);
}

IlacListe::~IlacListe()
{
    delete ui;
}

void IlacListe::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(6);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("BARKOD");
    basliklar.append("ILAC AD");
    basliklar.append("ETKEN MADDE");
    basliklar.append("FORM");
    basliklar.append("DOZAJ BILGISI");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0=new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->barkod()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->ad()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->etkenMadde()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(liste[i]->form()));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(tr("%1").arg(liste[i]->dozajBilgisi()));
        ui->tableWidget->setItem(i,5,hucre5);
    }
}

void IlacListe::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste=VERITABANI::vt().ilaclar().tumu();
    }else {
        liste=VERITABANI::vt().ilaclar().bul([this](IlacTablosu::VeriPointer d){
            return d->ad().toLower().contains(ui->leAra->text().toLower())||
                d->barkod().toLower().contains(ui->leAra->text().toLower())||
                d->etkenMadde().toLower().contains(ui->leAra->text().toLower())||
                QString::number(d->id()).contains(ui->leAra->text())||
                d->form().toLower().contains(ui->leAra->text().toLower());
        });
    }
    tabloguncelle();
}

void IlacListe::tablewidget_silmesecimi()
{
    bool secimvarmi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimvarmi);
    ui->btnBilgi->setEnabled(secimvarmi);
}

void IlacListe::on_btnSil_clicked()
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
            VERITABANI::vt().ilaclar().IdyeGoreSil(silinecekid);
        }
    }

}

void IlacListe::on_leAra_textChanged(const QString &arg1)
{
    ara();
}


void IlacListe::on_btnYeni_clicked()
{
    IlacEkle ilaceklefrm;
    auto ilac = VERITABANI::vt().ilaclar().olustur();
    ilaceklefrm.setVeri(ilac);
    ilaceklefrm.setAttribute(Qt::WA_QuitOnClose, false);
    auto cevap=ilaceklefrm.exec();
    if(cevap==QDialog::Accepted){
        ilac=ilaceklefrm.getVeri();
        if(ilac){
            VERITABANI::vt().ilaclar().ekle(ilac);
            QMessageBox::information(this,
                                     tr("Bilgilendirme"),
                                     tr("%1 adlı ilac eklendi.").arg(ilac->ad()));
        }else{
            QMessageBox::warning(this,
                                 tr("Hatalı Bilgi"),
                                 tr("Bu barkod numarası zaten başka bir ilaçta kayıtlı!"));
        }

    }

    ara();
}


void IlacListe::on_btnBilgi_clicked()
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

    ilacbilgi frm(id,this);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    frm.exec();

    tabloguncelle();
}

