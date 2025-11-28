#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileUtils.h"
#include "XMLValidator.h"
#include "XMLMinifier.h"
#include "Tree.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons to slots
    connect(ui->btnOkok, &QPushButton::clicked, this, &MainWindow::on_btnOk_Clicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MainWindow::on_btnCancel_Clicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
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

void MainWindow::on_browseButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select XML File",
        QDir::homePath(),            // default start folder
        "XML Files (*.xml);;All Files (*.*)"
    );

    if (!filePath.isEmpty()) {
        std::string content = readFileToString(filePath.toStdString());
    

    std::vector<Token> tokens = tokenizeXML(content);

    Tree<std::string>* tree = buildTree(tokens);
    tree->print_prettified();
    std::string out = "amr.xml";
    writeToFile(out,tree->getPrettifyingString());

    }
}
