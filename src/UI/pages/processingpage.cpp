#include "processingpage.h"
#include "ui_processingpage.h"

ProcessingPage::ProcessingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessingPage)
{
    ui->setupUi(this);
}

ProcessingPage::~ProcessingPage()
{
    delete ui;
}

void ProcessingPage::on_btnVerification_clicked()
{
    emit verificationClicked();
}

void ProcessingPage::on_btnFormatting_clicked()
{
    emit formattingClicked();
}

void ProcessingPage::on_btnMinify_clicked()
{
    emit minifyClicked();
}

void ProcessingPage::on_btnPrettify_clicked()
{
    emit prettifyClicked();
}

void ProcessingPage::on_btnConvertJson_clicked()
{
    emit convertToJsonClicked();
}

void ProcessingPage::on_btnCompress_clicked()
{
    emit compressClicked();
}

void ProcessingPage::on_btnDecompress_clicked()
{
    emit decompressClicked();
}

void ProcessingPage::on_btnBack_clicked()
{
    emit backToWelcomeClicked();
}