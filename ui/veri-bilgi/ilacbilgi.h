#pragma once

#include <QDialog>

namespace Ui {
class ilacbilgi;
}

class ilacbilgi : public QDialog
{
    Q_OBJECT

public:
    explicit ilacbilgi(quint32 id,QWidget *parent = nullptr);
    ~ilacbilgi();

    void loadData();

private slots:
    void on_btnDuzenle_clicked();

private:
    Ui::ilacbilgi *ui;
    quint32 veri_id;
};
