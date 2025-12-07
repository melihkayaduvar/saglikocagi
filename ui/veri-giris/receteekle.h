#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class receteekle;
}

class receteekle : public QDialog
{
    Q_OBJECT

public:
    explicit receteekle(QWidget *parent = nullptr);
    ~receteekle();

    ReceteTablosu::VeriPointer getVeri() const;
    void setVeri(const ReceteTablosu::VeriPointer &newVeri);

    void lwReceteKalemleri_silmesecimi();
    void cbZiyaret_guncelle();
    void cbIlac_guncelle();

private slots:
    void on_btnEkle_clicked();

    void on_btnSil_clicked();

private:
    Ui::receteekle *ui;

    ReceteTablosu::VeriPointer veri;
};
