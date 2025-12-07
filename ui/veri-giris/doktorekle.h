#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"
namespace Ui {
class doktorekle;
}

class doktorekle : public QDialog
{
    Q_OBJECT

public:
    explicit doktorekle(QWidget *parent = nullptr);
    ~doktorekle();

    DoktorTablosu::VeriPointer getVeri() const;
    void setVeri(const DoktorTablosu::VeriPointer &newVeri);

private:
    Ui::doktorekle *ui;

    DoktorTablosu::VeriPointer veri;
};
