#pragma once

#include <QWidget>
#include "../Veri/Araclar/TABLO_TANIMLARI.h"


namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_BtnDoktorGiris_clicked();

    void on_BtnYoneticiGiris_clicked();

private:
    Ui::login *ui;
};
