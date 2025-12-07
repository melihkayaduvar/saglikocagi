#pragma once

#include <QDialog>


namespace Ui {
class yenisifrebelirle;
}

class yenisifrebelirle : public QDialog
{
    Q_OBJECT

public:
    explicit yenisifrebelirle(quint32 id,QWidget *parent = nullptr);
    ~yenisifrebelirle();

    void accept();
private slots:
    void on_btnDegistir_clicked();

private:
    Ui::yenisifrebelirle *ui;
    quint32 veri_id;
};
