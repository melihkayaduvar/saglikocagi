#pragma once

#include <QDialog>

#include "../../Veri/Araclar/TABLO_TANIMLARI.h"

namespace Ui {
class tetkikekle;
}

class tetkikekle : public QDialog
{
    Q_OBJECT

public:
    explicit tetkikekle(QWidget *parent = nullptr);
    ~tetkikekle();

    TetkikTablosu::VeriPointer getVeri() const;
    void setVeri(const TetkikTablosu::VeriPointer &newVeri);

signals:
    void veriChanged();

private:
    Ui::tetkikekle *ui;

    TetkikTablosu::VeriPointer veri;
    Q_PROPERTY(TetkikTablosu::VeriPointer veri READ getVeri WRITE setVeri NOTIFY veriChanged FINAL)
};
