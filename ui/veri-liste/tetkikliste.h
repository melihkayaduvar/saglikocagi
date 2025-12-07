#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"
namespace Ui {
class TetkikListe;
}

class TetkikListe : public QDialog
{
    Q_OBJECT

public:
    explicit TetkikListe(QWidget *parent = nullptr);
    ~TetkikListe();
    void ara();
    void tabloguncelle();
    void tablewidget_silmesecimi();
    void duzenleTiklandi();
private slots:
    void on_btnSil_clicked();
    void on_leAra_textChanged(const QString &arg1);

    void on_btnEkle_clicked();

private:
    Ui::TetkikListe *ui;
    TetkikTablosu::VeriDizisi liste;
};
