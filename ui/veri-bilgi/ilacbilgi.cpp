#include "ilacbilgi.h"
#include "ui_ilacbilgi.h"
#include "../../Veri/veritabani.h"
#include "../veri-duzenleme/ilacduzenle.h"
#include <QMessageBox>

ilacbilgi::ilacbilgi(quint32 id,QWidget *parent)
    : QDialog(parent), veri_id(id)
    , ui(new Ui::ilacbilgi)
{
    ui->setupUi(this);
    loadData();
}

ilacbilgi::~ilacbilgi()
{
    delete ui;
}

void ilacbilgi::loadData()
{
    auto ilac = VERITABANI::vt().ilaclar().IdyeGoreAra(veri_id);
    ui->leAdi->setText(ilac->ad());
    ui->leBarkod->setText(ilac->barkod());
    ui->leDozaj->setText(ilac->dozajBilgisi());
    ui->leEtkenMad->setText(ilac->etkenMadde());
    ui->leForm->setText(ilac->form());
}

void ilacbilgi::on_btnDuzenle_clicked()
{
    ilacduzenle frm(veri_id, this);
    if(frm.exec()==QDialog::Accepted){
        auto guncelveri=frm.getVeri();
        VERITABANI::vt().ilaclar().duzenle(veri_id,[guncelveri](auto& veri){
            auto asilID=veri->id();
            *veri=*guncelveri;
            veri->setId(asilID);
        });
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 idli istenen hasta güncellendi.").arg(veri_id));

        loadData();
    }
}

