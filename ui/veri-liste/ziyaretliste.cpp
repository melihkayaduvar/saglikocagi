#include "ziyaretliste.h"
#include "ui/veri-liste/ui_ziyaretliste.h"

#include "../../Veri/veritabani.h"
#include <qmessagebox.h>
#include "../veri-giris/receteekle.h"
#include "../veri-giris//istenentetkikekle.h"

ziyaretliste::ziyaretliste(quint32 kid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ziyaretliste),
    m_kid(kid)
{
    ui->setupUi(this);

    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged ,this, &ziyaretliste::tablewidget_silmesecimi);
    connect(ui->btnReceteOlustur, &QPushButton::clicked,this,&ziyaretliste::receteolustur);
    connect(ui->btnTetkikIste, &QPushButton::clicked,this,&ziyaretliste::tetkikiste);
}

ziyaretliste::~ziyaretliste()
{
    delete ui;
}

void ziyaretliste::ara()
{
    liste.clear();
    if(m_kid==-1){
        if(ui->leAra->text().trimmed().isEmpty()){
            liste = VERITABANI::vt().ziyaretler().tumu();
        }else{
            liste = VERITABANI::vt().ziyaretler().bul([this](ZiyaretTablosu::VeriPointer d){
                auto doktor=VERITABANI::vt().doktorlar().IdyeGoreAra(d->doktorid());
                auto hasta=VERITABANI::vt().hastalar().IdyeGoreAra(d->hastaid());
                return QString::number(d->id()).contains(ui->leAra->text())||
                                                doktor->adi().toLower().contains(ui->leAra->text().toLower())||
                                                doktor->soyadi().toLower().contains(ui->leAra->text().toLower())||
                                                hasta->adi().toLower().contains(ui->leAra->text().toLower())||
                                                hasta->soyadi().toLower().contains(ui->leAra->text().toLower());
            });
        }
    }else{
        if(ui->leAra->text().trimmed().isEmpty()){
            liste = VERITABANI::vt().ziyaretler().bul([this](ZiyaretTablosu::VeriPointer d){
                return d->hastaid()==m_kid;
            });
        }else{
            liste = VERITABANI::vt().ziyaretler().bul([this](ZiyaretTablosu::VeriPointer d){
                auto doktor=VERITABANI::vt().doktorlar().IdyeGoreAra(d->doktorid());
                auto hasta=VERITABANI::vt().hastalar().IdyeGoreAra(d->hastaid());
                return hasta->id()==m_kid&&(QString::number(d->id()).contains(ui->leAra->text())||
                       doktor->adi().toLower().contains(ui->leAra->text().toLower())||
                       doktor->soyadi().toLower().contains(ui->leAra->text().toLower())||
                       hasta->adi().toLower().contains(ui->leAra->text().toLower())||
                       hasta->soyadi().toLower().contains(ui->leAra->text().toLower()));
            });
        }
    }
    tabloguncelle();
}

void ziyaretliste::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.count());
    ui->tableWidget->setColumnCount(7);
    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("Ziyaret Tarihi");
    basliklar.append("Şikayet");
    basliklar.append("Tanı");
    basliklar.append("Tedavi Notları");
    basliklar.append("Doktor");
    basliklar.append("Hasta");

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0 = new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1 = new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->tarihsaat().toString()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2 = new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->sikayet()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3 = new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->tani()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4 = new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(liste[i]->tedavinotlari()));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5 = new QTableWidgetItem;
        auto doktor = VERITABANI::vt().doktorlar().IdyeGoreAra(liste[i]->doktorid());
        hucre5->setText(tr("%1").arg(doktor->adi()+" "+doktor->soyadi()));
        ui->tableWidget->setItem(i,5,hucre5);
        QTableWidgetItem *hucre6 = new QTableWidgetItem;
        auto hasta = VERITABANI::vt().hastalar().IdyeGoreAra(liste[i]->hastaid());
        hucre6->setText(tr("%1").arg(hasta->adi()+" "+hasta->soyadi()));
        ui->tableWidget->setItem(i,6,hucre6);
    }
}
void ziyaretliste::tablewidget_silmesecimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnReceteOlustur->setEnabled(secimVarMi);
    ui->btnTetkikIste->setEnabled(secimVarMi);
    //ui->btnSil->setEnabled(secimVarMi);
}

void ziyaretliste::receteolustur()
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
    int satir = ui->tableWidget->currentRow();
    auto id = ui->tableWidget->item(satir,0)->text().toUInt();
    receteekle frm(id);
    frm.setAttribute(Qt::WA_QuitOnClose,false);
    frm.exec();
}

void ziyaretliste::tetkikiste()
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
    int satir = ui->tableWidget->currentRow();
    auto id = ui->tableWidget->item(satir,0)->text().toUInt();
    IstenenTetkikEkle frm(m_kid);
    frm.setAttribute(Qt::WA_QuitOnClose,false);
    frm.exec();
}
/*void ziyaretliste::on_btnSil_clicked()
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
            VERITABANI::vt().ziyaretler().IdyeGoreSil(silinecekid);
            auto silinecektetkik = VERITABANI::vt().istenentetkikler().bul([&silinecekid](IstenenTetkikTablosu::VeriPointer d){
                return d->ziyaretid()==silinecekid;
            });
            for(auto &i:silinecektetkik){
                VERITABANI::vt().istenentetkikler().IdyeGoreSil(i->id());
            }
            auto silinecekrecete = VERITABANI::vt().receteler().bul([&silinecekid](ReceteTablosu::VeriPointer d){
                return d->ziyaretid()==silinecekid;
            });
            for(auto &i:silinecekrecete){
                auto id=i->id();
                auto silinecekkalemler = VERITABANI::vt().recetekalemleri().bul([&id](ReceteKalemiTablosu::VeriPointer d){
                    return d->receteid()==id;
                });
                VERITABANI::vt().receteler().IdyeGoreSil(i->id());
                for (auto &j:silinecekkalemler){
                    VERITABANI::vt().recetekalemleri().IdyeGoreSil(j->id());
                }
            }
        }
    }
}*/

void ziyaretliste::on_leAra_textChanged(const QString &arg1)
{
    ara();
}
