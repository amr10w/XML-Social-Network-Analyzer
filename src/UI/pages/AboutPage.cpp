#include "aboutpage.h"
#include "ui_aboutpage.h" 
#include <QDesktopServices>
#include <QUrl>

AboutPage::AboutPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);

    // Force resize to ensure it's not too large
    resize(600, 600);
    // setMaximumHeight(500); // Also limit max height if needed

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutPage::~AboutPage()
{
    delete ui;
}

void AboutPage::on_btnTeam_clicked()
{
    emit teamPageRequested();
    this->close();
}