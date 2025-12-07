#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"
namespace Ui {
class HastaEkle;
}

class HastaEkle : public QDialog
{
    Q_OBJECT

public:
    explicit HastaEkle(QWidget *parent = nullptr);
    ~HastaEkle();

    HastaTablosu::VeriPointer getVeri() const;
    void setVeri(const HastaTablosu::VeriPointer &newVeri);

private slots:
    void on_btnAlerjilerEkle_clicked();

    void on_btnKronikHastaliklarEkle_clicked();

    void on_btnAlerjilerSil_clicked();

    void lwAlerjiler_silmesecimi();

    void on_btnKronikHastaliklarSil_clicked();

    void lwKronikHastaliklar_silmesecimi();

private:
    Ui::HastaEkle *ui;

    HastaTablosu::VeriPointer veri;
};
