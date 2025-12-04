#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AboutPage.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Create instances of your pages
    m_welcomePage = new WelcomePage(this);
    
    // 2. Add them to the stacked widget (defined in mainwindow.ui)
    ui->stackedWidget->addWidget(m_welcomePage);
    
    // (Example: Add a second page later)
    // m_processingPage = new ProcessingPage(this);
    // ui->stackedWidget->addWidget(m_processingPage);

    // 3. Connect signals from WelcomePage to MainWindow slots
    connect(m_welcomePage, &WelcomePage::startProcessingClicked, this, &MainWindow::onStartProcessingClicked);
    connect(m_welcomePage, &WelcomePage::aboutClicked, this, &MainWindow::onAboutClicked);

    // Start at index 0 (Welcome Page)
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchToPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::onStartProcessingClicked()
{
    qDebug() << "Switching to Processing Page...";
    // In the future: ui->stackedWidget->setCurrentWidget(m_processingPage);
}

void MainWindow::onAboutClicked()
{
    AboutPage aboutDialog(this);
    aboutDialog.exec(); // .exec() opens it as a modal dialog (blocks interaction with main window)
}