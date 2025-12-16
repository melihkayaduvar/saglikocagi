#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class ziyaretliste;
}

class ziyaretliste : public QDialog
{
    Q_OBJECT

public:
    explicit ziyaretliste(quint32 kid,QWidget *parent = nullptr);
    ~ziyaretliste();
    void ara();
    void tabloguncelle();
    void tablewidget_silmesecimi();
private slots:
    void on_leAra_textChanged(const QString &arg1);

private:
    Ui::ziyaretliste *ui;
    ZiyaretTablosu::VeriDizisi liste;
    quint32 m_kid;
};
