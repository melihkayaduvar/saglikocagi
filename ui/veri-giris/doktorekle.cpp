#include "doktorekle.h"
#include "ui/veri-giris/ui_doktorekle.h"
#include "../../Veri/veritabani.h"
#include <qregularexpression.h>

doktorekle::doktorekle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::doktorekle)
{
    ui->setupUi(this);
}

doktorekle::~doktorekle()
{
    delete ui;
}

DoktorTablosu::VeriPointer doktorekle::getVeri() const
{
    veri->setadi(ui->leAdi->text());
    veri->setsoyadi(ui->leSoyadi->text());
    QString masked = ui->leTel->text();
    QString unmasked = masked;
    unmasked.remove(QRegularExpression("[^0-9]"));
    unmasked="0"+unmasked;
    veri->settelefon(unmasked);
    auto x=ui->leDipno->text();
    auto sorgu=VERITABANI::vt().doktorlar().bul([this, &x](DoktorTablosu::VeriPointer d){
        return d->diplomaNo()==x;
    });
    if(!sorgu.isEmpty()){
        return nullptr;
    }
    veri->setdiplomano(ui->leDipno->text());
    veri->setuzmanlikAlani(ui->leUzmalani->text());

    return veri;
}

void doktorekle::setVeri(const DoktorTablosu::VeriPointer &newVeri)
{
    veri = newVeri;

    ui->leAdi->setText(veri->adi());
    ui->leSoyadi->setText(veri->soyadi());
    QString temiz=veri->telefon();
    if(temiz.length()==11){
        if(temiz.startsWith("0")){
            temiz.remove(0,1);
        }
        QString masked = QString("(%1) %2 %3 %4")
        .arg(temiz.mid(0, 3), temiz.mid(3, 3), temiz.mid(6, 2), temiz.mid(8, 2));  // Son 2 rakam

        ui->leTel->setText(masked);
    }else{
        ui->leTel->setText("");
    }
    ui->leDipno->setText(veri->diplomaNo());
    ui->leUzmalani->setText(veri->uzmanlikAlani());
}
