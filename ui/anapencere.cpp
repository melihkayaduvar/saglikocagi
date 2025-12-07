#include "anapencere.h"
#include "ui_anapencere.h"
#include "veri-giris/bulguekle.h"
#include "veri-giris/doktorekle.h"

#include "../Veri/veritabani.h"
#include "veri-giris/hastaekle.h"
#include "veri-giris/ilacekle.h"
#include "veri-giris/istenentetkikekle.h"
#include "veri-giris/receteekle.h"
#include "veri-giris/tetkikekle.h"
#include "veri-giris/ziyaretekle.h"
#include "veri-liste/bulguliste.h"
#include "veri-liste/doktorliste.h"
#include "veri-liste/hastaliste.h"
#include "veri-liste/ilacliste.h"
#include "veri-liste/istenentetkikliste.h"
#include "veri-liste/receteliste.h"
#include "veri-liste/tetkikliste.h"
#include "veri-liste/ziyaretliste.h"

#include <QMessageBox>
#include <QCloseEvent>

anapencere::anapencere(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::anapencere)
{
    ui->setupUi(this);

    ui->teEklenecekler->setReadOnly(true);
    toggleScrollArea(false);
    connect(ui->ButtonEklenecekler, &QPushButton::toggled, this, &anapencere::toggleScrollArea);
    ui->tabWidget->insertTab(0, new QWidget(), "");
    ui->tabWidget->insertTab(ui->tabWidget->count(), new QWidget(), "");
    // Sol ve sağa eklenen boş sekmeleri tıklanamaz yap
    QTabBar* tabBar = ui->tabWidget->tabBar();

    // 0. sekme (baştaki boş sekme)
    tabBar->setTabEnabled(0, false);

    // Son sekme (sondaki boş sekme)
    tabBar->setTabEnabled(tabBar->count() - 1, false);
    tabBar->setTabText(0, ""); // Sekme başlığını kaldır

}


anapencere::~anapencere()
{
    delete ui;
}

void anapencere::toggleScrollArea(bool checked)
{
    ui->scrollArea->setVisible(checked);
    ui->ButtonEklenecekler->setText(checked ? "Eklenecekler'i Gizle" : "Eklenecekler'i Göster");
}


void anapencere::on_action_k_triggered()
{
    close();
}


void anapencere::closeEvent(QCloseEvent *event)
{
    auto cevap = QMessageBox::question(this,
                                       tr("Çıkış Onayı"),
                                       tr("Çıkış Yapmak İstediğinize Eminmisiniz?"));
    if(cevap == QMessageBox::No){
        event->ignore();
    }
}

//Doktor
void anapencere::on_actiondoktorekle_triggered()
{
    doktorekle doktorekleform;
    auto doktor = VERITABANI::vt().doktorlar().olustur();
    doktorekleform.setVeri(doktor);

    auto cevap = doktorekleform.exec();
    if(cevap==QDialog::Accepted){
        doktor = doktorekleform.getVeri();
        VERITABANI::vt().doktorlar().ekle(doktor);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 %2 adlı doktor eklendi.").arg(doktor->adi(), doktor->soyadi()));
    }
}


void anapencere::on_actiondoktorlist_triggered()
{
    DoktorListe doktorlistefrm;
    doktorlistefrm.exec();
}

//Hasta
void anapencere::on_actionhastaekle_triggered()
{
    HastaEkle hastaekleform;
    auto hasta = VERITABANI::vt().hastalar().olustur();
    hastaekleform.setVeri(hasta);

    auto cevap = hastaekleform.exec();
    if(cevap==QDialog::Accepted){
        hasta=hastaekleform.getVeri();
        VERITABANI::vt().hastalar().ekle(hasta);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 %2 adlı hasta eklendi.").arg(hasta->adi(), hasta->soyadi()));
    }
}


void anapencere::on_actionhastalist_triggered()
{
    HastaListe hastalistefrm;
    hastalistefrm.exec();
}

//Ziyaret
void anapencere::on_actionziyaretekle_triggered()
{
    ZiyaretEkle ziyareteklefrm;
    auto ziyaret = VERITABANI::vt().ziyaretler().olustur();
    ziyareteklefrm.setVeri(ziyaret);

    auto cevap = ziyareteklefrm.exec();
    if(cevap==QDialog::Accepted){
        ziyaret=ziyareteklefrm.getVeri();
        VERITABANI::vt().ziyaretler().ekle(ziyaret);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 tarihli ziyaret eklendi.").arg(ziyaret->tarihsaat().toString()));
    }
}


void anapencere::on_actionziyaretlist_triggered()
{
    ziyaretliste ziyaretlistfrm;
    ziyaretlistfrm.exec();
}

//İstenen Tetkik
void anapencere::on_actionistetnentetkikekle_triggered()
{
    IstenenTetkikEkle istenentetkikfrm;
    //istenentetkikfrm.exec();
    auto istenentetkik =VERITABANI::vt().istenentetkikler().olustur();
    istenentetkikfrm.setVeri(istenentetkik);

    auto cevap = istenentetkikfrm.exec();
    if(cevap==QDialog::Accepted){
        istenentetkik=istenentetkikfrm.getVeri();
        VERITABANI::vt().istenentetkikler().ekle(istenentetkik);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 id'li tetkik talep edildi.").arg(istenentetkik->tetkikid()));
    }
}


void anapencere::on_actionistenentetkiklist_triggered()
{
    istenentetkikliste istenentetkiklistfrm;
    istenentetkiklistfrm.exec();
}

//Recete
void anapencere::on_actionreceteekle_triggered()
{
    receteekle recetefrm;
    auto recete=VERITABANI::vt().receteler().olustur();
    recetefrm.setVeri(recete);
    auto cevap=recetefrm.exec();
    if(cevap == QDialog::Accepted){
        recete = recetefrm.getVeri();
        VERITABANI::vt().receteler().ekle(recete);

        for(auto &kalem: recete->kalemler()){
            kalem->setReceteId(recete->id());
            VERITABANI::vt().recetekalemleri().ekle(kalem);
        }
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 id'li recete oluşturuldu.").arg(recete->id()));
    }
}


void anapencere::on_actionrecetelist_triggered()
{
    receteliste recetelistefrm;
    recetelistefrm.exec();
}
//İlac
void anapencere::on_actionilacekle_triggered()
{
    IlacEkle ilaceklefrm;
    auto ilac = VERITABANI::vt().ilaclar().olustur();
    ilaceklefrm.setVeri(ilac);

    auto cevap=ilaceklefrm.exec();
    if(cevap==QDialog::Accepted){
        ilac=ilaceklefrm.getVeri();
        VERITABANI::vt().ilaclar().ekle(ilac);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 adlı ilac eklendi.").arg(ilac->ad()));
    }
}


void anapencere::on_actionilaclist_triggered()
{
    IlacListe ilaclistfrm;
    ilaclistfrm.exec();
}
//Tetkik
void anapencere::on_actiontetkikekle_triggered()
{
    tetkikekle tetkikeklefrm;
    auto tetkik = VERITABANI::vt().tetkikler().olustur();
    tetkikeklefrm.setVeri(tetkik);

    auto cevap=tetkikeklefrm.exec();
    if(cevap==QDialog::Accepted){
        tetkik=tetkikeklefrm.getVeri();
        VERITABANI::vt().tetkikler().ekle(tetkik);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 adlı tetkik eklendi.").arg(tetkik->ad()));
    }
}


void anapencere::on_actiontetkiklist_triggered()
{
    TetkikListe tetkiklistfrm;
    tetkiklistfrm.exec();
}



void anapencere::on_actionbulguekle_triggered()
{
    bulguekle bulgufrm;
    auto bulgu = VERITABANI::vt().bulgular().olustur();
    bulgufrm.setVeri(bulgu);
    auto cevap=bulgufrm.exec();
    if(cevap==QDialog::Accepted){
        bulgu=bulgufrm.getVeri();
        VERITABANI::vt().bulgular().ekle(bulgu);
        QMessageBox::information(this,
                                 tr("Bilgilendirme"),
                                 tr("%1 idli bulgu eklendi").arg(bulgu->id()));
    }
}


void anapencere::on_actionbulgulist_triggered()
{
    bulguliste bulgulistfrm;
    bulgulistfrm.exec();
}

void anapencere::on_ButtonEklenecekler_clicked()
{

}

