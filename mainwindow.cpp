#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"matrix.h"
#include"matrixdatabase.h"
#include"const.h"
#include<QTextDocument>
#include<QTextBlock>
#include<QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Help->append(HELP_STR);
    read();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::MatrixInput() const {
    QTextDocument* document = ui->MatrixInput->document();
    QTextBlock textBlock;
    QString data;
    for(textBlock = document->begin();
        textBlock != document->end();
        textBlock = textBlock.next())
        if(textBlock.text() != "")
            data += textBlock.text().remove(QRegExp("\\s")) + "\n";
    return data;
}

QString MainWindow::MatrixName() const {
    QTextDocument* document = ui->MatrixName->document();
    QTextBlock textBlock;
    QString data;
    for(textBlock = document->begin();
        textBlock != document->end();
        textBlock = textBlock.next())
        data += textBlock.text().remove(QRegExp("\\s"));
    return data;
}

void MainWindow::on_submit_btn_clicked()
{
    QString nm = MatrixName();
    QString data = MatrixInput();
    Matrix* new_matrix = new Matrix(data);
    if(nm == "$reload$") {
        ui->MatrixSave->clear();
        delete new_matrix;
        for(int i = 0, length = mc.saves()->size();
            i < length; i++)
        {
            ui->MatrixSave->append(">>" + mc.saves()->at(i)->name() + ":");
            ui->MatrixSave->append(mc.saves()->at(i)->output_str());
        }
        return;
    }
    else if(nm.indexOf("$delete$") != -1) {
        QString n = nm.mid(nm.indexOf("$delete$") + 8).simplified();
        if(mc.saves()->del(n))
            ui->MatrixSave->append(">>" + n + " deleted\n");
        else
            ui->MatrixSave->append(">>No such Matrix called " +
                                   n + "\n");
        return;
    }
    ui->MatrixInput->clear();
    ui->MatrixName->clear();
    if(new_matrix->OK() != CORRECT) {
        ui->MatrixSave->append(">>Matrix " + nm + " is illegal\n");
        delete new_matrix;
        return;
    }
    if(mc.saves()->push(nm, new_matrix)) {
        ui->MatrixSave->append(">>" + nm + ":");
        ui->MatrixSave->append(new_matrix->output_str());
    }
    else {
        ui->MatrixSave->append(">>You have named a Matrix called: " + nm + "\n");
        delete new_matrix;
    }
}

void MainWindow::on_clear_btn_clicked()
{
    ui->MatrixInput->clear();
    ui->MatrixName->clear();
}

void MainWindow::on_view_clear_btn_clicked()
{
    ui->MatrixSave->clear();
    mc.saves()->clear();
}

void MainWindow::read() {
    QDir dir(FILE_DIR);
    QFileInfoList f_list = dir.entryInfoList();
    dir.setFilter(QDir::Files|QDir::NoDot|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    for(int i = 0;i < f_list.count();i++){
            QFileInfo f_info=f_list.at(i);
            if(f_info.fileName() == "." ||
               f_info.fileName() == "..")
                continue;
            Matrix* new_matrix = new Matrix(f_info.fileName(),1);
            if(mc.saves()->push(new_matrix->name(), new_matrix)) {
                if(new_matrix->OK() == CORRECT) {
                    ui->MatrixSave->append(">>" + new_matrix->name() + ":");
                    ui->MatrixSave->append(new_matrix->output_str());
                }
                else {
                    ui->MatrixSave->append(">>Matrix " + new_matrix->name() +
                                           " is illegal\n" +
                                           "  So it has been deleted\n");
                    QFile::remove(FILE_DIR + "//" + new_matrix->file_name());
                    mc.saves()->pop();
                }
            }
            else {
                ui->MatrixSave->append(">>You have named a Matrix called: " +
                                       new_matrix->name() + "\n");
                delete new_matrix;
            }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QTextDocument* document = ui->CalInput->document();
    QTextBlock textBlock;
    QString data;
    for(textBlock = document->begin();
        textBlock != document->end();
        textBlock = textBlock.next())
        if(textBlock.text() != "")
            data += textBlock.text();
    mc.calculate(data.remove(QRegExp("\\s")));
    ui->AnswerOutput->append(">>" + data + " =:");
    ui->AnswerOutput->append(mc.output_str() + "\n");
}

void MainWindow::on_cal_clear_btn2_clicked()
{
    ui->AnswerOutput->clear();
}

void MainWindow::on_cal_clear_btn1_clicked()
{
    ui->CalInput->clear();
}

void MainWindow::on_reload_btn_clicked()
{
    ui->MatrixSave->clear();
    for(int i = 0, length = mc.saves()->size();
        i < length; i++)
    {
        ui->MatrixSave->append(">>" + mc.saves()->at(i)->name() + ":");
        ui->MatrixSave->append(mc.saves()->at(i)->output_str());
    }
}
