#include "recetekalemiliste.h"
#include "ui/veri-liste/ui_recetekalemiliste.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

recetekalemiliste::recetekalemiliste(quint32 id, QWidget *parent)
    : QDialog(parent) , veri_id(id),
    ui(new Ui::recetekalemiliste)
{
    ui->setupUi(this);
    ara();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

recetekalemiliste::~recetekalemiliste()
{
    delete ui;
}

void recetekalemiliste::ara()
{
    liste.clear();
    if(ui->leAra->text().trimmed().isEmpty()){
        liste = VERITABANI::vt().recetekalemleri().bul([this](ReceteKalemiTablosu::VeriPointer d){
            return veri_id== d->receteid();
        });
    }else{
        liste= VERITABANI::vt().recetekalemleri().bul([this](ReceteKalemiTablosu::VeriPointer d){
            auto x = VERITABANI::vt().ilaclar().IdyeGoreAra(d->ilacid());
            return (veri_id==d->receteid())&&(
                        x->ad().toLower().contains(ui->leAra->text().toLower())||
                        x->barkod().contains(ui->leAra->text())||
                        QString::number(d->id()).contains(ui->leAra->text())
                    );
        });
    }
    loadData();
}

void recetekalemiliste::loadData()
{
    ui->tableWidget->setRowCount(liste.size());
    ui->tableWidget->setColumnCount(7);

    QStringList basliklar;
    basliklar.append("ID");
    basliklar.append("KULANIM SEKLİ");
    basliklar.append("DOZ");
    basliklar.append("PERIYOT");
    basliklar.append("ADET");
    basliklar.append("ILAC");
    basliklar.append("RECETE ID");

    for(auto i=0;i<liste.size();i++){
        QTableWidgetItem *hucre0 = new QTableWidgetItem;
        hucre0->setText(tr("%1").arg(liste[i]->id()));
        ui->tableWidget->setItem(i,0,hucre0);
        QTableWidgetItem *hucre1=new QTableWidgetItem;
        hucre1->setText(tr("%1").arg(liste[i]->kullanimSekli()));
        ui->tableWidget->setItem(i,1,hucre1);
        QTableWidgetItem *hucre2=new QTableWidgetItem;
        hucre2->setText(tr("%1").arg(liste[i]->doz()));
        ui->tableWidget->setItem(i,2,hucre2);
        QTableWidgetItem *hucre3=new QTableWidgetItem;
        hucre3->setText(tr("%1").arg(liste[i]->periyot()));
        ui->tableWidget->setItem(i,3,hucre3);
        QTableWidgetItem *hucre4 = new QTableWidgetItem;
        hucre4->setText(tr("%1").arg(liste[i]->adet()));
        ui->tableWidget->setItem(i,4,hucre4);
        auto ilac = VERITABANI::vt().ilaclar().IdyeGoreAra(liste[i]->ilacid());
        QTableWidgetItem *hucre5=new QTableWidgetItem;
        hucre5->setText(tr("Adı: %1 Barkod: %2").arg(ilac->ad(),ilac->barkod()));
        ui->tableWidget->setItem(i,5,hucre5);
        QTableWidgetItem *hucre6=new QTableWidgetItem;
        hucre6->setText(tr("%1").arg(liste[i]->receteid()));
        ui->tableWidget->setItem(i,6,hucre6);
    }

    ui->tableWidget->setHorizontalHeaderLabels(basliklar);
}

void recetekalemiliste::on_leAra_textChanged(const QString &arg1)
{
    ara();
}

