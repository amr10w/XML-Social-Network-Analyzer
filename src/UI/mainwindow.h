#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "welcomepage.h"
#include "teampage.h"
#include "processingpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Helper to switch pages
    void switchToPage(int index);

private slots:
    // Slots to handle signals from pages
    void onStartProcessingClicked();
    void onAboutClicked();
    void onTeamPageClicked();
    void onBackToWelcomeClicked();

private:
    Ui::MainWindow *ui;
    WelcomePage *m_welcomePage;
    TeamPage *m_teamPage;
    ProcessingPage *m_processingPage;
};

#endif // MAINWINDOW_H