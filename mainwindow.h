#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"calculator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void read();

    Calculator mc;

private slots:
    void on_submit_btn_clicked();

    void on_clear_btn_clicked();

    void on_view_clear_btn_clicked();

    void on_pushButton_clicked();

    void on_cal_clear_btn2_clicked();

    void on_cal_clear_btn1_clicked();

    void on_reload_btn_clicked();

    QString MatrixInput() const;

    QString MatrixName() const;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
