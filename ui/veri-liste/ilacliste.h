#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class IlacListe;
}

class IlacListe : public QDialog
{
    Q_OBJECT

public:
    explicit IlacListe(QWidget *parent = nullptr);
    ~IlacListe();

    void tabloguncelle();
    void ara();

    void tablewidget_silmesecimi();
private slots:
    void on_btnSil_clicked();
    void on_leAra_textChanged(const QString &arg1);

    void on_btnYeni_clicked();

    void on_btnBilgi_clicked();

private:
    Ui::IlacListe *ui;
    IlacTablosu::VeriDizisi liste;
};
