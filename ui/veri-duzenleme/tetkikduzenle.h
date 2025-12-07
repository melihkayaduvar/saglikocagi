#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class tetkikduzenle;
}

class tetkikduzenle : public QDialog
{
    Q_OBJECT

public:
    explicit tetkikduzenle(quint32 id,QWidget *parent = nullptr);
    ~tetkikduzenle();

    void loadData();
    TetkikTablosu::VeriPointer getVeri() const;

    void accept();
private slots:
    void on_btnKaydet_clicked();
private:
    Ui::tetkikduzenle *ui;
    quint32 veri_id;

    TetkikTablosu::VeriPointer veri;
};
