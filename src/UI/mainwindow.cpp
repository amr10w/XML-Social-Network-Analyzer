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
    m_teamPage = new TeamPage(this);
    m_processingPage = new ProcessingPage(this);

    // 2. Add them to the stacked widget (defined in mainwindow.ui)
    ui->stackedWidget->addWidget(m_welcomePage);
    ui->stackedWidget->addWidget(m_teamPage);
    ui->stackedWidget->addWidget(m_processingPage);
    
    // 3. Connect signals from WelcomePage to MainWindow slots
    connect(m_welcomePage, &WelcomePage::startProcessingClicked, this, &MainWindow::onStartProcessingClicked);
    connect(m_welcomePage, &WelcomePage::aboutClicked, this, &MainWindow::onAboutClicked);
    connect(m_welcomePage, &WelcomePage::teamPageClicked, this, &MainWindow::onTeamPageClicked);
    
    // Connect TeamPage back button to go to Welcome Page
    connect(m_teamPage, &TeamPage::backToWelcomeClicked, this, &MainWindow::onBackToWelcomeClicked);
    
    // Connect ProcessingPage back button to go to Welcome Page
    connect(m_processingPage, &ProcessingPage::backToWelcomeClicked, this, &MainWindow::onBackToWelcomeClicked);

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
    ui->stackedWidget->setCurrentWidget(m_processingPage);
}

void MainWindow::onAboutClicked()
{
    // Using the implementation from 'main' to show the actual dialog
    AboutPage aboutDialog(this);
    aboutDialog.exec(); // .exec() opens it as a modal dialog (blocks interaction with main window)
}

// These methods from 'TeamPage-Creation' are kept because they are connected in the constructor
void MainWindow::onTeamPageClicked()
{
    qDebug() << "Switching to Team Page...";
    ui->stackedWidget->setCurrentWidget(m_teamPage);
}

void MainWindow::onBackToWelcomeClicked()
{
    qDebug() << "Switching back to Welcome Page...";
    ui->stackedWidget->setCurrentWidget(m_welcomePage);
}