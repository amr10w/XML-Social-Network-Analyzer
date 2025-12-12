#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AboutPage.h" // For the dialog
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 1. Create instances of ALL pages ---
    m_welcomePage = new WelcomePage(this);
    m_teamPage = new TeamPage(this);
    m_processingPage = new ProcessingPage(this);
    
    m_verificationPage = new VerificationPage(this);
    m_minifyingPage = new MinifyingPage(this);
    m_postSearchPage = new PostSearch(this);
    m_socialPage = new SocialRelationsPage(this);
    m_graphPage = new GraphPage(this);
    m_formattingPage = new FormattingPage(this);
    m_jsonPage = new JsonConverterPage(this);
    m_compressPage = new CompressPage(this);
    m_influencePage = new InfluenceAndActivity(this);

    // --- 2. Add them to the stacked widget ---
    // The order here defines the index (0, 1, 2, 3...)
    ui->stackedWidget->addWidget(m_welcomePage);      // Index 0
    ui->stackedWidget->addWidget(m_teamPage);         // Index 1
    ui->stackedWidget->addWidget(m_processingPage);   // Index 2
    
    ui->stackedWidget->addWidget(m_verificationPage); // Index 3
    ui->stackedWidget->addWidget(m_minifyingPage);    // Index 4
    ui->stackedWidget->addWidget(m_postSearchPage);   // Index 5
    ui->stackedWidget->addWidget(m_socialPage);       // Index 6
    ui->stackedWidget->addWidget(m_graphPage);        // Index 7
    ui->stackedWidget->addWidget(m_formattingPage);   // Index 8
    ui->stackedWidget->addWidget(m_jsonPage);         // Index 9
    ui->stackedWidget->addWidget(m_compressPage);     // Index 10
    ui->stackedWidget->addWidget(m_influencePage);    // Index 11

    // --- 3. Connect Welcome Page Signals ---
    connect(m_welcomePage, &WelcomePage::startProcessingClicked, this, &MainWindow::onStartProcessingClicked);
    connect(m_welcomePage, &WelcomePage::aboutClicked, this, &MainWindow::onAboutClicked);
    connect(m_welcomePage, &WelcomePage::teamPageClicked, this, &MainWindow::onTeamPageClicked);

    // --- 4. Connect "Back" buttons (Assuming pages have a 'back' signal) ---
    // If your pages don't have this signal yet, you need to add it to them (like you did for TeamPage)
    connect(m_teamPage, &TeamPage::backToWelcomeClicked, this, &MainWindow::onBackToWelcomeClicked);
    connect(m_processingPage, &ProcessingPage::backToWelcomeClicked, this, &MainWindow::onBackToWelcomeClicked);
    
    // Example: Connecting Verification Page back button
    // connect(m_verificationPage, &VerificationPage::backClicked, this, &MainWindow::onBackToWelcomeClicked);

    connect(m_processingPage, &ProcessingPage::verificationClicked, this, &MainWindow::onGoToVerification);
    // Formatting button removed, Prettify handles this now
    connect(m_processingPage, &ProcessingPage::minifyClicked, this, &MainWindow::onGoToMinifying);
    connect(m_processingPage, &ProcessingPage::prettifyClicked, this, &MainWindow::onGoToFormatting); // Prettify uses FormattingPage
    connect(m_processingPage, &ProcessingPage::convertToJsonClicked, this, &MainWindow::onGoToJSON);  
    connect(m_processingPage, &ProcessingPage::graphClicked, this, &MainWindow::onGoToGraph);
    connect(m_processingPage, &ProcessingPage::socialClicked, this, &MainWindow::onGoToSocial);
    connect(m_processingPage, &ProcessingPage::postSearchClicked, this, &MainWindow::onGoToPostSearch);
    connect(m_processingPage, &ProcessingPage::compressClicked, this, &MainWindow::onGoToCompress);
    connect(m_processingPage, &ProcessingPage::influenceClicked, this, &MainWindow::onGoToInfluence);

    // --- Connect Tool Pages BACK to Welcome (or ProcessingPage) ---
    // Using Welcome as per current pattern, but could be ProcessingPage if desired
    connect(m_verificationPage, &VerificationPage::backClicked, this, &MainWindow::onBackToProcessingClicked);
    connect(m_formattingPage, &FormattingPage::backToHomeClicked, this, &MainWindow::onBackToProcessingClicked);
    connect(m_minifyingPage, &MinifyingPage::backToHomeClicked, this, &MainWindow::onBackToProcessingClicked);
    connect(m_jsonPage, &JsonConverterPage::backToHomeClicked, this, &MainWindow::onBackToProcessingClicked);
    connect(m_graphPage, &GraphPage::backClicked, this, &MainWindow::onBackToProcessingClicked); // Need to add signal to GraphPage
    connect(m_socialPage, &SocialRelationsPage::backClicked, this, &MainWindow::onBackToProcessingClicked);
    // PostSearch needs back signal too
    connect(m_postSearchPage, &PostSearch::backClicked, this, &MainWindow::onBackToProcessingClicked); // Need to add signal to PostSearch
    connect(m_compressPage, &CompressPage::backToHomeClicked, this, &MainWindow::onBackToProcessingClicked);
    connect(m_influencePage, &InfluenceAndActivity::backToHomeClicked, this, &MainWindow::onBackToProcessingClicked);

    // Start at Welcome Page
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Navigation Slots ---

void MainWindow::onStartProcessingClicked()
{
    // Go to the "Hub" page
    ui->stackedWidget->setCurrentWidget(m_processingPage);
}

void MainWindow::onAboutClicked()
{
    AboutPage aboutDialog(this);
    
    // Connect AboutPage signal to switch to TeamPage
    connect(&aboutDialog, &AboutPage::teamPageRequested, this, &MainWindow::onTeamPageClicked);
    
    aboutDialog.exec(); 
}

void MainWindow::onTeamPageClicked()
{
    ui->stackedWidget->setCurrentWidget(m_teamPage);
}

void MainWindow::onBackToWelcomeClicked()
{
    ui->stackedWidget->setCurrentWidget(m_welcomePage);
}

void MainWindow::onBackToProcessingClicked()
{
    ui->stackedWidget->setCurrentWidget(m_processingPage);
}

// --- Slots to open specific tools ---

void MainWindow::onGoToVerification()
{
    ui->stackedWidget->setCurrentWidget(m_verificationPage);
}

void MainWindow::onGoToMinifying()
{
    ui->stackedWidget->setCurrentWidget(m_minifyingPage);
}

void MainWindow::onGoToGraph()
{
    ui->stackedWidget->setCurrentWidget(m_graphPage);
}

void MainWindow::onGoToSocial()
{
    ui->stackedWidget->setCurrentWidget(m_socialPage);
}

void MainWindow::onGoToFormatting()
{
    ui->stackedWidget->setCurrentWidget(m_formattingPage);
}

void MainWindow::onGoToJSON()
{
    ui->stackedWidget->setCurrentWidget(m_jsonPage);
}

void MainWindow::onGoToPostSearch()
{
    ui->stackedWidget->setCurrentWidget(m_postSearchPage);
}

void MainWindow::onGoToCompress()
{
    ui->stackedWidget->setCurrentWidget(m_compressPage);
}

void MainWindow::onGoToInfluence()
{
    ui->stackedWidget->setCurrentWidget(m_influencePage);
}

