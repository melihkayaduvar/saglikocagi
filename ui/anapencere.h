#pragma once

#include <QMainWindow>

namespace Ui {
class anapencere;
}

class anapencere : public QMainWindow
{
    Q_OBJECT

public:
    explicit anapencere(QWidget *parent = nullptr);
    ~anapencere();
    void toggleScrollArea(bool checked);
private slots:
    void on_action_k_triggered();

    void on_actiondoktorekle_triggered();

    void on_actiondoktorlist_triggered();

    void on_actionhastaekle_triggered();

    void on_actionhastalist_triggered();

    void on_actionziyaretekle_triggered();

    void on_actionziyaretlist_triggered();

    void on_actionilacekle_triggered();

    void on_actionilaclist_triggered();

    void on_actionistetnentetkikekle_triggered();

    void on_actionistenentetkiklist_triggered();

    void on_actionreceteekle_triggered();

    void on_actionrecetelist_triggered();

    void on_actiontetkikekle_triggered();

    void on_actiontetkiklist_triggered();

    void on_actionbulguekle_triggered();

    void on_actionbulgulist_triggered();

    void on_ButtonEklenecekler_clicked();

private:
    Ui::anapencere *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
