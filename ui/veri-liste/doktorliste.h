#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class DoktorListe;
}

class DoktorListe : public QDialog
{
    Q_OBJECT

public:
    explicit DoktorListe(QWidget *parent = nullptr);
    ~DoktorListe();
    void ara();
    void tabloguncelle();
    void tablewidget_secimi();

private slots:
    void on_btnSil_clicked();

    void on_leAra_textChanged(const QString &arg1);

    void on_BtnDoktorOlustur_clicked();

    void on_btnYeniSifre_clicked();

private:
    Ui::DoktorListe *ui;

    DoktorTablosu::VeriDizisi liste;
};
