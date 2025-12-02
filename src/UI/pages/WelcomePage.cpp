#include "welcomepage.h"
#include "ui_welcomepage.h"
#include <QResource> 

WelcomePage::WelcomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePage)
{
    // CRITICAL FIX: Force the "resources.qrc" file to be loaded.
    // Since this is in a Static Library, the linker might skip the images 
    // without this line.
    Q_INIT_RESOURCE(resources);

    ui->setupUi(this);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}

void WelcomePage::on_btnStart_clicked()
{
    emit startProcessingClicked();
}

void WelcomePage::on_btnAbout_clicked()
{
    emit aboutClicked();
}