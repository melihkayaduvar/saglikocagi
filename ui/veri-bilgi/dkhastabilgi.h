#pragma once

#include <QDialog>

namespace Ui {
class dkHastaBilgi;
}

class dkHastaBilgi : public QDialog
{
    Q_OBJECT

public:
    explicit dkHastaBilgi(quint32 id,QWidget *parent = nullptr);
    ~dkHastaBilgi();

    void loadData();

private slots:
    void on_btnDuzenle_clicked();

private:
    Ui::dkHastaBilgi *ui;
    quint32 veri_id;
};
