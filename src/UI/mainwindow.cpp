#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnOk, &QPushButton::clicked, this, &MainWindow::on_btnOk_Clicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MainWindow::on_btnCancel_Clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOk_Clicked()
{
    QMessageBox::information(this, "Info", "Ok button clicked");
}

void MainWindow::on_btnCancel_Clicked()
{
    QMessageBox::warning(this, "Warning", "Cancel button clicked");
}