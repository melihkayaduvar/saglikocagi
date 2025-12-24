#include "yenisifrebelirle.h"
#include "ui/veri-duzenleme/ui_yenisifrebelirle.h"
#include <qmessagebox.h>
#include "../../Veri/veritabani.h"

yenisifrebelirle::yenisifrebelirle(quint32 id, QWidget *parent)
    : QDialog(parent), veri_id(id),
    ui(new Ui::yenisifrebelirle)
{
    ui->setupUi(this);
    auto doktor=VERITABANI::vt().doktorlar().IdyeGoreAra(veri_id);
    ui->leDoktorAdi->setText(doktor->adi()+" "+doktor->soyadi());
    ui->leDoktorId->setText(QString::number(doktor->id()));
}

yenisifrebelirle::~yenisifrebelirle()
{
    delete ui;
}

void yenisifrebelirle::accept()
{
    auto answer = QMessageBox::question(this,
                                        tr("Onay"),
                                        tr("Şifreyi Değiştirmeyi Onaylıyormusunuz?"));
    if (answer==QMessageBox::Yes){
        auto liste = VERITABANI::vt().doktorsifreleri().bul([this](LoginPassTablosu::VeriPointer d){
            return d->kid()==veri_id;
        });
        if (liste.isEmpty()) {
            QMessageBox::warning(this, tr("Hata"), tr("Şifre kaydı bulunamadı."));
            return;
        }
        VERITABANI::vt().doktorsifreleri().duzenle(liste[0]->id(),[this](LoginPassTablosu::VeriPointer d){
            d->setSifre(ui->leYeniSifre->text());
        });
        QDialog::accept();
    }
}

void yenisifrebelirle::on_btnDegistir_clicked()
{
    yenisifrebelirle::accept();
}

