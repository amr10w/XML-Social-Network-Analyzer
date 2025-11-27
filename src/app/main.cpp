#include "mainwindow.h"
#include "CLICommands.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // CASE 1 — user passed CLI arguments
    if(argc>1)
    {
        CLICommands cli;
        return cli.handle(argc, argv);   // run CLI only

        // xml_editor verify -i test.xml
        // xml_editor mini -i in.xml -o out.xml
        // xml_editor json -i test.xml -o test.json



    }
    else{
    // CASE 2 — no arguments → launch GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    }

    
}