#include "tetkikekle.h"
#include "ui/veri-giris/ui_tetkikekle.h"

tetkikekle::tetkikekle(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tetkikekle)
{
    ui->setupUi(this);
}

tetkikekle::~tetkikekle()
{
    delete ui;
}

TetkikTablosu::VeriPointer tetkikekle::getVeri() const
{
    veri->setAd(ui->leAdi->text());
    veri->setAciklama(ui->teAciklama->toPlainText());
    veri->setNormalDegerlerErkek((ui->leNormaldegerErkek->text()));
    veri->setNormalDegerlerKadın(ui->leNormaldegerKadin->text());

    return veri;
}

void tetkikekle::setVeri(const TetkikTablosu::VeriPointer &newVeri)
{
    veri = newVeri;

    ui->leAdi->setText(veri->ad());
    ui->teAciklama->setText(veri->aciklama());
    ui->leNormaldegerErkek->setText(veri->normalDegerlerErkek());
    ui->leNormaldegerKadin->setText(veri->normalDegerlerKadın());
}
