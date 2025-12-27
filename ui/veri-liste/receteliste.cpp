#include "receteliste.h"
#include "ui/veri-liste/ui_receteliste.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

receteliste::receteliste(quint32 kid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::receteliste),
    m_kid(kid)
{
    ui->setupUi(this);

    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged ,this, &receteliste::tablewidget_silmesecimi);
    connect(ui->btnReceteKalemleri, &QPushButton::clicked, this, &receteliste::recetekalemleri);
}

receteliste::~receteliste()
{
    delete ui;
}
void receteliste::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().receteler().bul([this](ReceteTablosu::VeriPointer d){
            return d->ziyaretid()==m_kid;
        });
    }else{
        liste = VERITABANI::vt().receteler().bul([this](ReceteTablosu::VeriPointer d){
            auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(d->ziyaretid());
            auto id=ziyaret->hastaid();
            auto hasta= VERITABANI::vt().hastalar().bul([&id](HastaTablosu::VeriPointer d){
                return  d->id()==id;
            });
            return d->ziyaretid()==m_kid&&(QString::number(d->id()).contains(ui->leAra->text())||
                   QString::number(d->ziyaretid()).contains(ui->leAra->text())||
                   hasta[0]->adi().toLower().contains(ui->leAra->text().toLower())||
                   hasta[0]->soyadi().toLower().contains(ui->leAra->text().toLower())||
                   d->tarih().toString().toLower().contains(ui->leAra->text()));
        });
    }
    tabloguncelle();
}
void receteliste::tabloguncelle()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(6);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("RECETE TARIHI");
    basliklar.append("GECERLİLİK SURESI(GUN)");
    basliklar.append("HASTA");
    basliklar.append("ZIYARET ID");
    basliklar.append("RECETE KALEMLERI");

    for(auto i=0;i<liste.size();i++){
        quint32 id=liste[i]->id();
        auto kalemler=VERITABANI::vt().recetekalemleri().bul([&id](ReceteKalemiTablosu::VeriPointer kalem){
            return kalem->receteid() == id;
        });

        QStringList idler;
        for(auto i=0;i<kalemler.size();i++){
            idler.append(QString::number(kalemler[i]->id()));
        }
        QString idler_n=idler.join(", ");
        QTableWidgetItem *hucre0 = new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->tarih().toString()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->gecerlilikSuresi()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3 =new QTableWidgetItem;
        auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(m_kid);
        auto hastaid=ziyaret->hastaid();
        auto hasta=VERITABANI::vt().hastalar().bul([&hastaid](HastaTablosu::VeriPointer d){
            return d->id()==hastaid;
        });
        hucre3->setText(tr("%1").arg(hasta[0]->adi()+" "+hasta[0]->soyadi()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4=new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(m_kid));
        ui->tableWidget->setItem(i,4,hucre4);
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(tr("%1").arg(idler_n));
        ui->tableWidget->setItem(i,5,hucre5);
    }

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);
}


void receteliste::tablewidget_silmesecimi()
{
    bool secimVarMi = !ui->tableWidget->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
    ui->btnReceteKalemleri->setEnabled(secimVarMi);
}

void receteliste::recetekalemleri()
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

    recetekalemiliste frm(id, this);
    frm.setAttribute(Qt::WA_QuitOnClose, false);
    frm.exec();
    tabloguncelle();
}

void receteliste::on_btnSil_clicked()
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
            VERITABANI::vt().receteler().IdyeGoreSil(silinecekid);
            auto recetekalemleri = VERITABANI::vt().recetekalemleri().bul([&silinecekid](ReceteKalemiTablosu::VeriPointer d){
                return d->receteid()==silinecekid;
            });
            for(auto &i:recetekalemleri){
                VERITABANI::vt().recetekalemleri().IdyeGoreSil(i->id());
            }
        }

    }
}

void receteliste::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

