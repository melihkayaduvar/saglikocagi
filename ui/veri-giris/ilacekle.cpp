#include "ilacekle.h"
#include "ui/veri-giris/ui_ilacekle.h"
#include <qregularexpression.h>
#include "../../Veri/veritabani.h"
#include <QMessageBox>

IlacEkle::IlacEkle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IlacEkle)
{
    ui->setupUi(this);
}

IlacEkle::~IlacEkle()
{
    delete ui;
}

IlacTablosu::VeriPointer IlacEkle::getVeri() const
{
    QString masked = ui->leBarkod->text();
    QString unmasked = masked;
    unmasked.remove(QRegularExpression("[^0-9]"));
    auto sorgu=VERITABANI::vt().ilaclar().bul([this, &unmasked](IlacTablosu::VeriPointer d){
        return d->barkod()==unmasked;
    });
    if(!sorgu.isEmpty()){
        return nullptr;
    }
    veri->setBarkod(unmasked);
    veri->setAd(ui->leAdi->text());
    veri->setEtkenMadde(ui->leEtkenMad->text());
    veri->setForm(ui->leForm->text());
    veri->setDozajBilgisi(ui->leDozaj->text());

    return veri;
}

void IlacEkle::setVeri(const IlacTablosu::VeriPointer &newVeri)
{
    veri = newVeri;

    QString temiz = veri->barkod();
    if(temiz.length()==7){
        QString masked= QString("%1 %2 %3").arg(temiz.mid(0,1),temiz.mid(1,3),temiz.mid(4,3));
    }
    ui->leBarkod->setText(tr("%1").arg(veri->barkod()));
    ui->leAdi->setText(tr("%1").arg(veri->ad()));
    ui->leEtkenMad->setText(tr("%1").arg(veri->etkenMadde()));
    ui->leForm->setText(tr("%1").arg(veri->form()));
    ui->leDozaj->setText(tr("%1").arg(veri->dozajBilgisi()));
}
