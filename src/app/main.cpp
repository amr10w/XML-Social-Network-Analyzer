#include "mainwindow.h"
#include "VerificationPage.h" // <--- Include this
#include "CLICommands.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // CASE 1 — user passed CLI arguments
    if(argc > 1)
    {
        CLICommands cli;
        return cli.handle(argc, argv);
    }
    else
    {
        // CASE 2 — no arguments → launch GUI
        QApplication a(argc, argv);

        a.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: white; }");
        // 1. Create and show the Main Window
        MainWindow w;
        w.show();

        // 2. Create and show the Verification Window alongside it
        VerificationPage v;
        v.show();

        return a.exec();
    }
}