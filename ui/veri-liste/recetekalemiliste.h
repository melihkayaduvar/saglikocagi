#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class recetekalemiliste;
}

class recetekalemiliste : public QDialog
{
    Q_OBJECT

public:
    explicit recetekalemiliste(quint32 id,QWidget *parent = nullptr);
    ~recetekalemiliste();

    void ara();
    void loadData();
private slots:
    void on_leAra_textChanged(const QString &arg1);

private:
    Ui::recetekalemiliste *ui;
    quint32 veri_id;

    ReceteKalemiTablosu::VeriDizisi liste;
};
