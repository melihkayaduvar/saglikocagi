#pragma once

#include <QMainWindow>

namespace Ui {
class doktorarayuzu;
}

class doktorarayuzu : public QMainWindow
{
    Q_OBJECT
signals:
    void oturumKapatildi();
public:
    explicit doktorarayuzu(quint32 dokid, QWidget *parent = nullptr);
    ~doktorarayuzu();
    void duyurulariYenile();

private slots:
    void on_btnHastalar_clicked();

    void on_btnZiyaret_clicked();

    void on_actCikis_triggered();

private:
    Ui::doktorarayuzu *ui;
    quint32 m_doktorID;
};
