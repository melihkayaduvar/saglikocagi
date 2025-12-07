#pragma once

#include <QDialog>

#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class ZiyaretEkle;
}

class ZiyaretEkle : public QDialog
{
    Q_OBJECT

public:
    explicit ZiyaretEkle(QWidget *parent = nullptr);
    ~ZiyaretEkle();

    ZiyaretTablosu::VeriPointer getVeri() const;
    void setVeri(const ZiyaretTablosu::VeriPointer &newVeri);

    void DoktorComboBoxVeri();
    void HastaComboBoxVeri();

private:
    Ui::ZiyaretEkle *ui;

    ZiyaretTablosu::VeriPointer veri;
};
