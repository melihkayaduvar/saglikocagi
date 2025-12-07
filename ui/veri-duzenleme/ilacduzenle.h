#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class ilacduzenle;
}

class ilacduzenle : public QDialog
{
    Q_OBJECT

public:
    explicit ilacduzenle(quint32 id,QWidget *parent = nullptr);
    ~ilacduzenle();

    void loadData();
    void accept();

    IlacTablosu::VeriPointer getVeri() const;

private slots:
    void on_btnKaydet_clicked();

private:
    Ui::ilacduzenle *ui;

    quint32 veri_id;
    IlacTablosu::VeriPointer veri;
};
