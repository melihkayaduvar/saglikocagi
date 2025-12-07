#pragma once

#include <QMainWindow>

namespace Ui {
class yoneticiarayuzu;
}

class yoneticiarayuzu : public QMainWindow
{
    Q_OBJECT

public:
    explicit yoneticiarayuzu(quint32 yonid, QWidget *parent = nullptr);
    ~yoneticiarayuzu();

    void yenile();
    void duyurulariYenile();
    QString enCokYazilanIlac();
signals:
    void oturumKapatildi();
private slots:
    void on_BtnDoktor_clicked();

    void on_BtnHasta_clicked();

    void on_BtnIlac_clicked();

    void on_BtnTetkik_clicked();

    void on_BtnDuyuru_clicked();

    void on_btnTumuSil_clicked();

    void on_btnDuyuruSil_clicked();

    void on_leID_textChanged(const QString &arg1);

    void on_actExit_triggered();

private:
    Ui::yoneticiarayuzu *ui;
    quint32 m_yoneticiID;
};
