#include "CompressPage.h"
#include "../../core/FileUtils.h"
// This include now contains the required declaration for compressXMLString
#include "../../core/XML_Compress.h" 
#include "ui_CompressPage.h" // The generated header file
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>

// Helper macro to wrap the newly declared backend function
extern std::string compressXMLString(const std::string& xmlContent);


CompressPage::CompressPage(QWidget *parent)
    : QScrollArea(parent)
    , ui(new Ui::CompressPage)
    , originalSize(0)
    , compressedSize(0) 
{
    ui->setupUi(this);
    // setFixedSize(900, 750);
    
    // Initial UI Setup 
    if (ui->statsContainer) ui->statsContainer->setVisible(false);
    if (ui->outputLabel) ui->outputLabel->setVisible(false);
    if (ui->outputTextEdit) ui->outputTextEdit->setVisible(false);
    if (ui->downloadButton) ui->downloadButton->setVisible(false);
    
    if (ui->inputTextEdit) {
        ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    }
    
    // Connect signals 
    connect(ui->backButton, &QPushButton::clicked, this, &CompressPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &CompressPage::onBrowseFile);
    connect(ui->minifyButton, &QPushButton::clicked, this, &CompressPage::onCompressXML); 
    connect(ui->downloadButton, &QPushButton::clicked, this, &CompressPage::onDownload);
}

CompressPage::~CompressPage()
{
    delete ui;
}

void CompressPage::onBackToOperations()
{
    // this->close();
    emit backToHomeClicked();
}

void CompressPage::onBrowseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse XML File"),
        QString(),
        tr("XML Files (*.xml);;All Files (*)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    QTextStream in(&file);
    inputXML = in.readAll();
    file.close();
    
    currentFilePath = fileName;
    ui->inputTextEdit->setPlainText(inputXML);
    
    // Hide output when new file is loaded (using statsContainer)
    ui->statsContainer->setVisible(false);
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
}

void CompressPage::onCompressXML()
{
    inputXML = ui->inputTextEdit->toPlainText();
    
    if (inputXML.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        // Calculate original size
        originalSize = inputXML.toUtf8().size();
        
        // Convert QString to std::string for the utility function
        std::string xmlContentStd = inputXML.toStdString();
        
        std::string compressedXmlStd = compressXMLString(xmlContentStd); 
        
        // Convert the result back to QString
        outputXML = QString::fromStdString(compressedXmlStd);
        
        // Calculate compressed size
        compressedSize = outputXML.toUtf8().size(); 
        
        // Display in output text edit
        ui->outputTextEdit->setPlainText(outputXML);
        
        // Update statistics
        updateStatistics();
        
        // Show output section
        updateOutputVisibility();
        
        // Scroll to output
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setValue(0);
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to compress XML: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while compressing XML");
    }
}


void CompressPage::updateStatistics()
{
    // FIX 1: Change minifiedSizeLabel to compressedSizeLabel 
    // to match the final .ui file structure.
    ui->originalSizeLabel->setText(QString::number(originalSize) + " bytes");
    ui->compressedSizeLabel->setText(QString::number(compressedSize) + " bytes"); 
    
    qint64 savedBytes = originalSize - compressedSize;
    double percentage = originalSize > 0 ? 
        ((double)savedBytes * 100.0 / originalSize) : 0.0;
    
    ui->reductionLabel->setText(QString::number(percentage, 'f', 2) + "%");
}

void CompressPage::updateOutputVisibility()
{
    bool hasOutput = !outputXML.isEmpty();
    // FIX 2: Ensure we consistently use statsContainer, not statsWidget
    ui->statsContainer->setVisible(hasOutput); 
    ui->outputLabel->setVisible(hasOutput);
    ui->outputTextEdit->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}

void CompressPage::onDownload()
{
    if (outputXML.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No compressed XML to download!");
        return;
    }
    
    QString defaultFileName = "compressed.comp"; // Changed default file name
    
    // Check if the user loaded a file previously
    if (!currentFilePath.isEmpty()) {
        QFileInfo info(currentFilePath);
        // Set the default name to 'original_name.comp'
        defaultFileName = info.completeBaseName() + ".comp"; 
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Compressed XML"),
        defaultFileName, // Use the new default file name
        tr("Compressed Files (*.comp);;All Files (*)") // Changed file filter description
    );
    
    if (saveFileName.isEmpty())
        return;
    
    QFile outFile(saveFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot save file!");
        return;
    }
    
    QTextStream out(&outFile);
    out << outputXML;
    outFile.close();
}

void CompressPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
}