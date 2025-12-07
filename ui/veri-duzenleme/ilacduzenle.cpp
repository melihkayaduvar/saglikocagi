#include "ilacduzenle.h"
#include "ui/veri-duzenleme/ui_ilacduzenle.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>
ilacduzenle::ilacduzenle(quint32 id, QWidget *parent)
    : QDialog(parent),  veri_id(id)
    , ui(new Ui::ilacduzenle)
{
    ui->setupUi(this);
    loadData();
}

ilacduzenle::~ilacduzenle()
{
    delete ui;
}

void ilacduzenle::loadData()
{
    auto asilveri = VERITABANI::vt().ilaclar().IdyeGoreAra(veri_id);
    if (!veri) {
        qWarning() << "Veri bulunamadı. ID:" << veri_id;
        return;
    }
    auto veri = VERITABANI::vt().ilaclar().olustur();
    *veri=*asilveri;
    ui->leAdi->setText(veri->ad());
    ui->leBarkod->setText(veri->barkod());
    ui->leDozaj->setText(veri->dozajBilgisi());
    ui->leEtkenMad->setText(veri->etkenMadde());
    ui->leForm->setText(veri->form());

    ui->leAdi->setEnabled(false);
    ui->leBarkod->setEnabled(false);
}

void ilacduzenle::accept()
{
    auto answer = QMessageBox::question(this,
                                        tr("Onay"),
                                        tr("Düzenlemeyi kaydetmek istiyormusunuz?"));
    if (answer==QMessageBox::Yes){
        QDialog::accept();

    }
}

void ilacduzenle::on_btnKaydet_clicked()
{
    ilacduzenle::accept();
}

IlacTablosu::VeriPointer ilacduzenle::getVeri() const
{
    if (!veri) {
        qWarning() << "HATA: veri bulunamadı!";
        return nullptr;
    }
    veri->setDozajBilgisi(ui->leDozaj->text());
    veri->setEtkenMadde(ui->leEtkenMad->text());
    veri->setForm(ui->leForm->text());
    return veri;
}

