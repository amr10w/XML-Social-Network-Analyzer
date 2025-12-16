#include "CompressPage.h"
#include "../../core/FileUtils.h"
#include "../../core/Compress.h" 
#include "ui_CompressPage.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>


CompressPage::CompressPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompressPage)
    , originalSize(0)
    , compressedSize(0) 
{
    ui->setupUi(this);
    
    if (ui->statsContainer) ui->statsContainer->setVisible(false);
    if (ui->outputLabel) ui->outputLabel->setVisible(false);
    if (ui->downloadButton) ui->downloadButton->setVisible(false);
    
    if (ui->inputTextEdit) {
        ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    }
    
    connect(ui->backButton, &QPushButton::clicked, this, &CompressPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &CompressPage::onBrowseFile);
    connect(ui->compressButton, &QPushButton::clicked, this, &CompressPage::onCompress); 
    connect(ui->downloadButton, &QPushButton::clicked, this, &CompressPage::onDownload);
}

CompressPage::~CompressPage()
{
    delete ui;
}

void CompressPage::onBackToOperations()
{
    emit backToHomeClicked();
}

void CompressPage::onBrowseFile()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse XML/JSON File"),
        documentsPath,
        tr("XML/JSON Files (*.xml *.json)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    QTextStream in(&file);
    inputQString = in.readAll();
    file.close();
    
    currentFilePath = fileName;
    ui->inputTextEdit->setPlainText(inputQString);
    ui->statsContainer->setVisible(false);
    ui->outputLabel->setVisible(false);
    ui->downloadButton->setVisible(false);
}

void CompressPage::onCompress()
{
    inputQString = ui->inputTextEdit->toPlainText();
    
    if (inputQString.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        originalSize = inputQString.toUtf8().size();
        
        std::string input = inputQString.toStdString();
        
        BPE compressor;

        auto data = compressor.compress(input);

        auto dataString = compressor.to_string(data);

        outputComp = QByteArray::fromStdString(dataString);
        
        compressedSize = outputComp.size(); 
        
        updateStatistics();
        
        updateOutputVisibility();
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to compress file: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while compression");
    }
}


void CompressPage::updateStatistics()
{
    ui->originalSizeLabel->setText(QString::number(originalSize) + " bytes");
    ui->compressedSizeLabel->setText(QString::number(compressedSize) + " bytes"); 
    
    qint64 savedBytes = originalSize - compressedSize;
    double percentage = originalSize > 0 ? 
        ((double)savedBytes * 100.0 / originalSize) : 0.0;
    
    ui->reductionLabel->setText(QString::number(percentage, 'f', 2) + "%");
}

void CompressPage::updateOutputVisibility()
{
    bool hasOutput = !outputComp.isEmpty();
    ui->statsContainer->setVisible(hasOutput); 
    ui->outputLabel->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}

void CompressPage::onDownload()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (outputComp.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No compressed XML to download!");
        return;
    }
    
    QString defaultFileName = documentsPath + "/compressed.comp";
    
    if (!currentFilePath.isEmpty()) {
        QFileInfo info(currentFilePath);
        defaultFileName = info.path() + "/" + info.completeBaseName() + ".comp"; 
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Compressed File"),
        defaultFileName,
        tr("Compressed Files (*.comp)")
    );
    
    if (saveFileName.isEmpty())
        return;
    
    QFile outFile(saveFileName);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot save file!");
        return;
    }
    
    outFile.write(outputComp);
}

void CompressPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
}