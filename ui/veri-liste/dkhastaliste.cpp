#include "dkhastaliste.h"
#include "ui/veri-liste/ui_dkhastaliste.h"
#include "../veri-bilgi/hastabilgi.h"
#include "../../Veri/veritabani.h"
#include "ziyaretliste.h"
#include <qmessagebox.h>
dkHastaListe::dkHastaListe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dkHastaListe)
{
    ui->setupUi(this);

    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged ,this, &dkHastaListe::tablewidget_silmesecimi);
    connect(ui->btnBilgi, &QPushButton::clicked, this, &dkHastaListe::bilgiTiklandi);
    connect(ui->btnZiyaret, &QPushButton::clicked, this, &dkHastaListe::ziyaretTiklandi);

}

dkHastaListe::~dkHastaListe()
{
    delete ui;
}

void dkHastaListe::bilgiTiklandi()
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

    HastaBilgi frm(id,this);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    frm.exec();

    tabloguncelle();
}

void dkHastaListe::ziyaretTiklandi()
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

    ziyaretliste frm(id,this);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    frm.exec();

    tabloguncelle();
}

void dkHastaListe::ara(){
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().hastalar().bul([](HastaTablosu::VeriPointer d){
            return true;
        });
    }else {
        liste = VERITABANI::vt().hastalar().bul([this](HastaTablosu::VeriPointer d){
            return d->adi().toLower().contains(ui->leAra->text().toLower())||
                   d->soyadi().toLower().contains(ui->leAra->text().toLower())||
                   QString::number(d->id()).contains(ui->leAra->text())||
                   d->tckimlik().toLower().contains(ui->leAra->text().toLower());
        });
    }
    tabloguncelle();
}
void dkHastaListe::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.count());
    ui->tableWidget->setColumnCount(11);
    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("ADI");
    basliklar.append("SOYADI");
    basliklar.append("CINSIYET");
    basliklar.append("DOGUM TARİHİ");
    basliklar.append("TC KIMLIK");
    basliklar.append("TELEFON");
    basliklar.append("ADRES");
    basliklar.append("KAN GRUBU");
    basliklar.append("ALERJILER");
    basliklar.append("KRONIK HASTALIKLAR");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        quint32 id=liste[i]->id();
        auto ziyaretler=VERITABANI::vt().ziyaretler().bul([id](ZiyaretTablosu::VeriPointer ziyaret){
            return ziyaret->hastaid() == id;
        });

        QTableWidgetItem *hucre0 = new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1 = new QTableWidgetItem;
        hucre1->setText(liste[i]->adi());
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2 = new QTableWidgetItem;
        hucre2->setText(liste[i]->soyadi());
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3 = new QTableWidgetItem;
        CinsiyetEnum cinsiyet = liste[i]->cinsiyet();
        switch (cinsiyet) {
        case CinsiyetEnum::ERKEK:
            hucre3->setText("Erkek");
            break;
        case CinsiyetEnum::KADIN:
            hucre3->setText("Kadın");
            break;
        case CinsiyetEnum::BELIRTILMEMIS:
            hucre3->setText("BELIRTILMEMIS");
            break;
        }
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(liste[i]->dogumTarihi().toString("dd.MM.yyyy"));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(liste[i]->tckimlik());
        ui->tableWidget->setItem(i,5,hucre5);
        QTableWidgetItem *hucre6=new QTableWidgetItem;
        hucre6->setText(liste[i]->telefon());
        ui->tableWidget->setItem(i,6,hucre6);
        QTableWidgetItem *hucre7=new QTableWidgetItem;
        hucre7->setText(liste[i]->adres());
        ui->tableWidget->setItem(i,7,hucre7);
        QTableWidgetItem *hucre8=new QTableWidgetItem;
        hucre8->setText(liste[i]->kanGrubu());
        ui->tableWidget->setItem(i,8,hucre8);
        QTableWidgetItem *hucre9=new QTableWidgetItem;
        hucre9->setText(liste[i]->alerjiler().join(", "));
        ui->tableWidget->setItem(i,9,hucre9);
        QTableWidgetItem *hucre10=new QTableWidgetItem;
        hucre10->setText(liste[i]->kronikHastaliklar().join(", "));
        ui->tableWidget->setItem(i,10,hucre10);
    }
}
void dkHastaListe::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

void dkHastaListe::tablewidget_silmesecimi()
{
    bool secimvarmi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnBilgi->setEnabled(secimvarmi);
    ui->btnZiyaret->setEnabled(secimvarmi);
}

