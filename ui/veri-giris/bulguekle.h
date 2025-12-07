#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class bulguekle;
}

class bulguekle : public QDialog
{
    Q_OBJECT

public:
    explicit bulguekle(QWidget *parent = nullptr);
    ~bulguekle();

    void hastaara();
    void hastatabloguncelle();

    void ziyaretara();
    void ziyarettabloguncelle();

    BulguTablosu::VeriPointer getVeri() const;
    void setVeri(BulguTablosu::VeriPointer newVeri);

    void Hastatable();
    void accept();
private slots:
    void on_leHastaBul_textChanged(const QString &arg1);

    void on_leZiyaretBul_textChanged(const QString &arg1);

    void on_btnKaydet_clicked();

private:
    Ui::bulguekle *ui;


    HastaTablosu::VeriDizisi hastaliste;
    ZiyaretTablosu::VeriDizisi ziyaretliste;
    BulguTablosu::VeriPointer veri;
};
