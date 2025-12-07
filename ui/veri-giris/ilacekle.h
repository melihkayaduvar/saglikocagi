#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class IlacEkle;
}

class IlacEkle : public QDialog
{
    Q_OBJECT

public:
    explicit IlacEkle(QWidget *parent = nullptr);
    ~IlacEkle();

    IlacTablosu::VeriPointer getVeri() const;
    void setVeri(const IlacTablosu::VeriPointer &newVeri);

signals:
    void veriChanged();

private:
    Ui::IlacEkle *ui;

    IlacTablosu::VeriPointer veri;
    Q_PROPERTY(IlacTablosu::VeriPointer veri READ getVeri WRITE setVeri NOTIFY veriChanged FINAL)
};
