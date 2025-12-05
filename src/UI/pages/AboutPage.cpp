#include "aboutpage.h"
#include "ui_aboutpage.h" 
#include <QDesktopServices>
#include <QUrl>

AboutPage::AboutPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutPage::~AboutPage()
{
    delete ui;
}

void AboutPage::on_btnTeam_clicked()
{
    
}