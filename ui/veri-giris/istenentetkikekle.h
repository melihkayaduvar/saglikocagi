#pragma once

#include <QDialog>
#include <qlistwidget.h>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class IstenenTetkikEkle;
}

class IstenenTetkikEkle : public QDialog
{
    Q_OBJECT

public:
    explicit IstenenTetkikEkle(quint32 kid, QWidget *parent = nullptr);
    ~IstenenTetkikEkle();

    IstenenTetkikTablosu::VeriPointer getVeri() const;
    void setVeri(const IstenenTetkikTablosu::VeriPointer &newVeri);

    void tabloguncelle();
    void onItemChanged(QListWidgetItem *changedItem);

    void ziyaretcomboboxveri();
    void accept();

private slots:
    void on_btnKaydet_clicked();

private:
    Ui::IstenenTetkikEkle *ui;

    IstenenTetkikTablosu::VeriPointer veri;
    quint32 m_kid;

};
