#include "DecompressPage.h"
#include "ui_DecompressPage.h"
#include "../../core/Decompress.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>

DecompressPage::DecompressPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecompressPage)
    , originalSize(0)
    , decompressedSize(0) 
{
    ui->setupUi(this);
    
    updateOutputVisibility(false);
    
    if (ui->inputTextEdit) {
        ui->inputTextEdit->setPlaceholderText("Use the 'Browse Compressed File' button to load a .comp file...");
        ui->inputTextEdit->setReadOnly(true); 
    }
    
    connect(ui->backButton, &QPushButton::clicked, this, &DecompressPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &DecompressPage::onBrowseFile);
    connect(ui->decompressButton, &QPushButton::clicked, this, &DecompressPage::onDecompress); 
    connect(ui->downloadButton, &QPushButton::clicked, this, &DecompressPage::onDownload);
}

DecompressPage::~DecompressPage()
{
    delete ui;
}

void DecompressPage::onBackToOperations()
{
    emit backToProcessingClicked();
}

void DecompressPage::onBrowseFile()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse Compressed BPE File"),
        documentsPath,
        tr("Compressed BPE Files (*.comp)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    compressedDataBytes = file.readAll();
    file.close();
    
    currentFilePath = fileName;
    
    QFileInfo info(fileName);
    QString fileSummary = QString("File Loaded:\nName: %1\nSize: %2 bytes\nPath: %3")
                          .arg(info.fileName())
                          .arg(compressedDataBytes.size())
                          .arg(info.absoluteFilePath());
                          
    ui->inputTextEdit->setPlainText(fileSummary);

    fileExtention = (compressedDataBytes[0] == 0)? ".xml" : ".json";
    
    updateOutputVisibility(false);
    outputQString.clear();
}

void DecompressPage::onDecompress()
{
    if (compressedDataBytes.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please load a compressed .comp file first!");
        return;
    }
    
    try {
        originalSize = compressedDataBytes.size();
        
        BPE decompressor;

        auto data = decompressor.from_string(compressedDataBytes.toStdString());

        auto decompressed = decompressor.decompress(data);
        
        outputQString = QString::fromStdString(decompressed);
        
        decompressedSize = outputQString.toUtf8().size(); 
        
        ui->outputTextEdit->setPlainText(outputQString);
        
        updateStatistics();
        updateOutputVisibility(true);
        
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setValue(0);
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to decompress file: %1").arg(e.what()));
        updateOutputVisibility(false);
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while decompressing XML");
        updateOutputVisibility(false);
    }
}


void DecompressPage::updateStatistics()
{
    ui->originalSizeLabel->setText(QString::number(originalSize) + " bytes");
    ui->decompressedSizeLabel->setText(QString::number(decompressedSize) + " bytes"); 
    
    qint64 savedBytes = decompressedSize - originalSize;
    double ratio = originalSize > 0 ? 
        ((double)originalSize * 100.0 / decompressedSize) : 0.0;
    
    ui->reductionLabel->setText(QString::number(ratio, 'f', 2) + "%");
}

void DecompressPage::updateOutputVisibility(bool visible)
{
    ui->statsContainer->setVisible(visible); 
    ui->outputLabel->setVisible(visible);
    ui->outputTextEdit->setVisible(visible);
    ui->downloadButton->setVisible(visible);
}

void DecompressPage::onDownload()
{
    if (outputQString.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No decompressed file to download!");
        return;
    }

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString defaultFileName = documentsPath + QString::fromStdString("/decompressed" + fileExtention); 
    
    if (!currentFilePath.isEmpty()) {
        QFileInfo info(currentFilePath);
        defaultFileName = info.path() + "/" + info.completeBaseName() + QString::fromStdString(fileExtention); 
    }
    
    const char* fileExtenionText = (fileExtention == ".xml")? "XML Files (*.xml)" : "JSON Files (*.json)";

    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Decompressed File"),
        defaultFileName, 
        tr(fileExtenionText)
    );
    
    if (saveFileName.isEmpty())
        return;
    
    QFile outFile(saveFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot save file!");
        return;
    }
    
    QTextStream out(&outFile);
    out << outputQString;
    outFile.close();
}

void DecompressPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
   
}