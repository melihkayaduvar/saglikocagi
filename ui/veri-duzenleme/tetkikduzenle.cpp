#include "tetkikduzenle.h"
#include "ui/veri-duzenleme/ui_tetkikduzenle.h"
#include "../../Veri/veritabani.h"
#include <qmessagebox.h>

tetkikduzenle::tetkikduzenle(quint32 id,QWidget *parent)
    : QDialog(parent),  veri_id(id),
    ui(new Ui::tetkikduzenle)
{
    ui->setupUi(this);
    loadData();
}

tetkikduzenle::~tetkikduzenle()
{
    delete ui;
}

void tetkikduzenle::loadData()
{
    veri = VERITABANI::vt().tetkikler().IdyeGoreAra(veri_id);
    if (!veri) {
        qWarning() << "Veri bulunamadı. ID:" << veri_id;
        return;
    }
    ui->leAdi->setText(veri->ad());
    ui->teAciklama->setText(veri->aciklama());
    ui->leNormaldegerErkek->setText(veri->normalDegerlerErkek());
    ui->leNormaldegerKadin->setText(veri->normalDegerlerKadın());
}

TetkikTablosu::VeriPointer tetkikduzenle::getVeri() const
{
    if (!veri) {
        qWarning() << "HATA: veri bulunamadı!";
        return nullptr;
    }
    veri->setNormalDegerlerErkek(ui->leNormaldegerErkek->text());
    veri->setNormalDegerlerKadın(ui->leNormaldegerKadin->text());
    return veri;
}

void tetkikduzenle::accept()
{
    auto answer = QMessageBox::question(this,
                                        tr("Onay"),
                                        tr("Düzenlemeyi kaydetmek istiyormusunuz?"));
    if (answer==QMessageBox::Yes){
        QDialog::accept();

    }
}

void tetkikduzenle::on_btnKaydet_clicked()
{
    tetkikduzenle::accept();
}
