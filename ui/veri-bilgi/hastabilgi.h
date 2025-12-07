#pragma once

#include <QDialog>

namespace Ui {
class HastaBilgi;
}

class HastaBilgi : public QDialog
{
    Q_OBJECT

public:
    explicit HastaBilgi(quint32 id,QWidget *parent = nullptr);
    ~HastaBilgi();

    void loadData();

private slots:
    void on_btnDuzenle_clicked();

private:
    Ui::HastaBilgi *ui;
    quint32 veri_id;
};
