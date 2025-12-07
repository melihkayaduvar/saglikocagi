#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class receteliste;
}

class receteliste : public QDialog
{
    Q_OBJECT

public:
    explicit receteliste(QWidget *parent = nullptr);
    ~receteliste();

    void tabloguncelle();
    void ara();
    void tablewidget_silmesecimi();
    void recetekalemleri();
private slots:
    void on_btnSil_clicked();

    void on_leAra_textChanged(const QString &arg1);

private:
    Ui::receteliste *ui;
    ReceteTablosu::VeriDizisi liste;
};
