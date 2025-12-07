#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class bulguliste;
}

class bulguliste : public QDialog
{
    Q_OBJECT

public:
    explicit bulguliste(QWidget *parent = nullptr);
    ~bulguliste();
    void ara();
    void tabloguncelle();
    void silmesecimi();

private slots:
    void on_btnSil_clicked();

    void on_leAra_textChanged(const QString &arg1);

private:
    Ui::bulguliste *ui;

    BulguTablosu::VeriDizisi liste;
};
