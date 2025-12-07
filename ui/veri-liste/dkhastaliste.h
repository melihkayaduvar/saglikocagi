#pragma once

#include <QDialog>
#include <QStringList>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class dkHastaListe;
}

class dkHastaListe : public QDialog
{
    Q_OBJECT

public:
    explicit dkHastaListe(QWidget *parent = nullptr);
    ~dkHastaListe();

    void tabloguncelle();
    void ara();

    void tablewidget_silmesecimi();
    void duzenleTiklandi();
    void bilgiTiklandi();
private slots:
    void on_leAra_textChanged(const QString &arg1);
private:
    Ui::dkHastaListe *ui;
    HastaTablosu::VeriDizisi liste;
};
