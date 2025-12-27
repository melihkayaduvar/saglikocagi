#pragma once

#include <QDialog>
#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class istenentetkikliste;
}

class istenentetkikliste : public QDialog
{
    Q_OBJECT

public:
    explicit istenentetkikliste(quint32 kid,QWidget *parent = nullptr);
    ~istenentetkikliste();

    void ara();
    void tabloguncelle();
    void duzenleTiklandi();
    void tablewidget_silmesecimi();
private slots:
    void on_btnSil_clicked();

    void on_leAra_textChanged(const QString &arg1);

private:
    Ui::istenentetkikliste *ui;
    IstenenTetkikTablosu::VeriDizisi liste;
    quint32 m_kid;
};
