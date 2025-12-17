#include "mainwindow.h"
#include "VerificationPage.h"
#include "CompressPage.h"
#include "DecompressPage.h"
#include "minifyingPage.h"
#include "postsearch.h"
#include "social_relations_page.h"
#include "influence_activity.h"

#include <QApplication>
#include <graphPage.h>

int main(int argc, char *argv[])
{
  qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
  // CASE 2 — no arguments → launch GUI
  QApplication a(argc, argv);

  a.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: white; }");
  a.setWindowIcon(QIcon(":/new/Prefix1/images/ilovexml.png"));

  // Create and show the Main Window ONLY
  MainWindow w;
  w.show();
  return a.exec();
}