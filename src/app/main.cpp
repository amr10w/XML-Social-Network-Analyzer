#include "mainwindow.h"
#include "VerificationPage.h"
#include "minifyingPage.h" 
#include "postsearch.h" 
#include "CLICommands.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // CASE 1 — user passed CLI arguments
    if (argc > 1)
    {
        CLICommands cli;

        // Documentation from 'main' branch
        // xml_editor verify -i test.xml
        // xml_editor mini -i in.xml -o out.xml
        // xml_editor json -i test.xml -o test.json
        // xml_editor compress -i sample.xml -o sample.comp
        // xml_editor decompress -i sample.comp -o sample_decompressed.xml

        return cli.handle(argc, argv);
    }
    else
    {
        // CASE 2 — no arguments → launch GUI
        QApplication a(argc, argv);

        a.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: white; }");
        
        // 1. Create and show the Main Window (From 'main')
        // MainWindow w;
        // w.show();

        // 2. Create and show the Verification Window (From 'main')
        // VerificationPage v;
        // v.show();

        // 3. Create and show the Minifying Page (From 'formatting-page')
        // Renamed to 'm' to avoid conflict with MainWindow 'w'
        // MinifyingPage m;
        // m.show();
       PostSearch P;
         P.show();

        return a.exec();
    }
}