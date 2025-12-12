#include "processingpage.h"
#include "ui_processingpage.h"

ProcessingPage::ProcessingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessingPage)
{
    ui->setupUi(this);

    // Programmatically add buttons for Graph, Social, and PostSearch
    QFont btnFont;
    btnFont.setPointSize(11);
    btnFont.setBold(true);

    // Graph Button
    QPushButton* btnGraph = new QPushButton("Graph Analysis\nVisualize detailed network graphs", this);
    btnGraph->setMinimumSize(0, 100);
    btnGraph->setFont(btnFont);
    // Explicitly add to layout at Row 3, Column 1
    ui->gridLayout->addWidget(btnGraph, 3, 1);
    
    // Social Button
    QPushButton* btnSocial = new QPushButton("Social Relations\nAnalyze mutual friends and suggestions", this);
    btnSocial->setMinimumSize(0, 100);
    btnSocial->setFont(btnFont);
    // Explicitly add to layout at Row 4, Column 0
    ui->gridLayout->addWidget(btnSocial, 4, 0);

    // PostSearch Button
    QPushButton* btnPostSearch = new QPushButton("Post Search\nSearch posts by topics or words", this);
    btnPostSearch->setMinimumSize(0, 100);
    btnPostSearch->setFont(btnFont);
    // Explicitly add to layout at Row 4, Column 1
    ui->gridLayout->addWidget(btnPostSearch, 4, 1);

    // Connect them
    connect(btnGraph, &QPushButton::clicked, this, &ProcessingPage::on_btnGraph_clicked);
    connect(btnSocial, &QPushButton::clicked, this, &ProcessingPage::on_btnSocial_clicked);
    connect(btnPostSearch, &QPushButton::clicked, this, &ProcessingPage::on_btnPostSearch_clicked);
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

void ProcessingPage::on_btnGraph_clicked()
{
    emit graphClicked();
}

void ProcessingPage::on_btnSocial_clicked()
{
    emit socialClicked();
}

void ProcessingPage::on_btnPostSearch_clicked()
{
    emit postSearchClicked();
}

void ProcessingPage::on_btnBack_clicked()
{
    emit backToWelcomeClicked();
}