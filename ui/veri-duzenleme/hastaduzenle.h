#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class hastaduzenle;
}

class hastaduzenle : public QDialog
{
    Q_OBJECT

public:
    explicit hastaduzenle(quint32 id,QWidget *parent = nullptr);
    ~hastaduzenle();

    void loadData();
    HastaTablosu::VeriPointer getVeri() const;

    void lwAlerjiler_silmesecimi();
    void lwKronikHastaliklar_silmesecimi();
    void accept();
private slots:
    void on_btnAlerjilerEkle_clicked();
    void on_btnKronikHastaliklarEkle_clicked();
    void on_btnAlerjilerSil_clicked();
    void on_btnKronikHastaliklarSil_clicked();
    void on_btnKaydet_clicked();

private:
    Ui::hastaduzenle *ui;
    quint32 veri_id;

    HastaTablosu::VeriPointer veri;
};
