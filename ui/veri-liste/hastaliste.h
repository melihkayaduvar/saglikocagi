#pragma once

#include <QDialog>
#include <QStringList>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class HastaListe;
}

class HastaListe : public QDialog
{
    Q_OBJECT

public:
    explicit HastaListe(QWidget *parent = nullptr);
    ~HastaListe();

    void tabloguncelle();
    void ara();

    void tablewidget_silmesecimi();
    void duzenleTiklandi();
    void bilgiTiklandi();
private slots:
    void on_btnSil_clicked();

    void on_leAra_textChanged(const QString &arg1);

    void on_btnHastaOlustur_clicked();

private:
    Ui::HastaListe *ui;
    HastaTablosu::VeriDizisi liste;
};
