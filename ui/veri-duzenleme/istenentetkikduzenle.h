#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class istenentetkikduzenle;
}

class istenentetkikduzenle : public QDialog
{
    Q_OBJECT

public:
    explicit istenentetkikduzenle(quint32 &id, QWidget *parent = nullptr);
    ~istenentetkikduzenle();

    void loadData();
    void onYeniDurumChanged(const QString& yeniDurum);

    IstenenTetkikTablosu::VeriPointer getVeri() const;

    void accept();

private slots:
    void on_btnKaydet_clicked();

private:
    quint32 veri_id;
    Ui::istenentetkikduzenle *ui;

    IstenenTetkikTablosu::VeriPointer veri;
};
