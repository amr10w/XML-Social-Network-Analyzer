#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// 1. Include ALL your page headers here
#include "welcomepage.h"
#include "teampage.h"
#include "processingpage.h"
#include "VerificationPage.h"
#include "minifyingPage.h"
#include "postsearch.h"
#include "social_relations_page.h"
#include "graphPage.h"
#include "formattingPage.h"
#include "json_converter_page.h"
#include "CompressPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void switchToPage(int index);

private slots:
    // Slots for Welcome Page
    void onStartProcessingClicked();
    void onAboutClicked();
    void onTeamPageClicked();

    // Slot to go back home (can be reused by all pages)
    void onBackToWelcomeClicked();
    
    // Slot to go back to Processing Hub
    void onBackToProcessingClicked();

    // Slots to switch to specific tools (You will connect these to buttons in ProcessingPage later)
    void onGoToVerification();
    void onGoToMinifying();
    void onGoToGraph();
    void onGoToSocial();
    void onGoToFormatting();
    void onGoToJSON();
    void onGoToPostSearch();
    void onGoToCompress();
    // ... add others as needed

private:
    Ui::MainWindow *ui;

    // 2. Declare pointers for ALL pages
    WelcomePage *m_welcomePage;
    TeamPage *m_teamPage;
    ProcessingPage *m_processingPage;
    
    VerificationPage *m_verificationPage;
    MinifyingPage *m_minifyingPage; 
    PostSearch *m_postSearchPage;
    SocialRelationsPage *m_socialPage;
    GraphPage *m_graphPage;
    FormattingPage *m_formattingPage;
    JsonConverterPage *m_jsonPage;
    CompressPage *m_compressPage;
};

#endif // MAINWINDOW_H