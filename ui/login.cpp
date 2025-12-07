#include "login.h"
#include "doktorarayuzu.h"
#include "ui_login.h"
#include "../Veri/veritabani.h"
#include "yoneticiarayuzu.h"
#include <qmessagebox.h>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    ui->leDoktorSifre->setEchoMode(QLineEdit::Password);
    ui->leYoneticiSifre->setEchoMode(QLineEdit::Password);
    //VERITABANI::vt().geriyukle();
}

login::~login()
{
    delete ui;
}


void login::on_BtnDoktorGiris_clicked()
{
    if (ui->leDoktorId->text().isEmpty() || ui->leDoktorSifre->text().isEmpty()) {
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Lütfen tüm alanları doldurun!"));
        return;
    }
    auto kid=ui->leDoktorId->text().toUInt();
    auto x=VERITABANI::vt().doktorsifreleri().bul([kid](LoginPassTablosu::VeriPointer d){
        return d->kid()==kid;
    });
    if(x.isEmpty()){
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Girdiğiniz Doktor İd'si Geçersiz!"));
        return;
    }
    if(x[0]->sifre()=="-1"){
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Yöneticinizden Sistem Şifresi Talep Ediniz!"));
        return;
    }
    if(ui->leDoktorSifre->text()==x[0]->sifre()){
        VERITABANI::vt().setAktifkid(kid);
        auto frm = new doktorarayuzu(kid,this);
        connect(frm, &doktorarayuzu::oturumKapatildi, this, &login::show);
        connect(frm, &doktorarayuzu::oturumKapatildi, frm, &doktorarayuzu::deleteLater);
        frm->show();
        this->hide();
        ui->leDoktorSifre->clear();
        ui->leYoneticiSifre->clear();
    }else {
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Şifre hatalı!"));
    }
}

void login::on_BtnYoneticiGiris_clicked()
{
    if (ui->leYoneticiKullaniciAdi->text().isEmpty() || ui->leYoneticiSifre->text().isEmpty()) {
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Lütfen tüm alanları doldurun!"));
        return;
    }
    auto kid=ui->leYoneticiKullaniciAdi->text().toUInt();
    auto x=VERITABANI::vt().yoneticisifreleri().bul([kid](LoginPassTablosu::VeriPointer d){
        return d->kid()==kid;
    });
    if(x.isEmpty()){
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Girdiğiniz Yönetici İd'si Geçersiz!"));
        return;
    }
    if(ui->leYoneticiSifre->text()==x[0]->sifre()){
        VERITABANI::vt().setAktifkid(kid);
        auto frm = new yoneticiarayuzu(kid,this);
        connect(frm, &yoneticiarayuzu::oturumKapatildi, this, &login::show);
        connect(frm, &yoneticiarayuzu::oturumKapatildi, frm, &yoneticiarayuzu::deleteLater);
        frm->show();
        this->hide();
        ui->leYoneticiSifre->clear();
        ui->leDoktorSifre->clear();
    }else {
        QMessageBox::warning(this,
                             tr("%1").arg("Uyarı"),
                             tr("%1").arg("Şifre hatalı!"));
    }
}

