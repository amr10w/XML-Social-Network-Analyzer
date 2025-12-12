#include "DecompressPage.h"
#include "ui_DecompressPage.h" // The generated header file
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>
#include "../../core/BPE_Bridge.h"

// Utility function we assume exists, wrapping the BPE::load_from_file and BPE::decompress
// We pass the raw compressed bytes (including the dictionary header) and get the XML text.
// NOTE: You must implement this bridge function in a utility file that links BPE.h/cpp.
extern std::string decompressxmlBytes(const QByteArray& compressedBytes); 


DecompressPage::DecompressPage(QWidget *parent)
    : QScrollArea(parent)
    , ui(new Ui::DecompressPage)
    , originalSize(0)
    , decompressedSize(0) 
{
    ui->setupUi(this);
    setFixedSize(900, 750);
    
    // Initial UI Setup 
    updateOutputVisibility(false);
    
    if (ui->inputTextEdit) {
        ui->inputTextEdit->setPlaceholderText("Use the 'Browse Compressed File' button to load a .comp file...");
        // Disable manual pasting, as compressed data is typically binary/non-text
        ui->inputTextEdit->setReadOnly(true); 
    }
    
    // Connect signals 
    connect(ui->backButton, &QPushButton::clicked, this, &DecompressPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &DecompressPage::onBrowseFile);
    connect(ui->decompressButton, &QPushButton::clicked, this, &DecompressPage::onDecompressXML); 
    connect(ui->downloadButton, &QPushButton::clicked, this, &DecompressPage::onDownload);
}

DecompressPage::~DecompressPage()
{
    delete ui;
}

void DecompressPage::onBackToOperations()
{
    emit close(); // Use close() or a custom signal to navigate back
}

void DecompressPage::onBrowseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse Compressed BPE File"),
        QString(),
        tr("Compressed BPE Files (*.comp);;All Files (*)") // Filter for .comp
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    // Read compressed file as raw binary data
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    compressedDataBytes = file.readAll();
    file.close();
    
    currentFilePath = fileName;
    
    // Display file info in the text edit area
    QFileInfo info(fileName);
    QString fileSummary = QString("File Loaded:\nName: %1\nSize: %2 bytes\nPath: %3")
                          .arg(info.fileName())
                          .arg(compressedDataBytes.size())
                          .arg(info.absoluteFilePath());
                          
    ui->inputTextEdit->setPlainText(fileSummary);
    
    // Clear previous output/stats
    updateOutputVisibility(false);
    outputXML.clear();
}

void DecompressPage::onDecompressXML()
{
    // Check if a compressed file has been loaded into the QByteArray buffer
    if (compressedDataBytes.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please load a compressed .comp file first!");
        return;
    }
    
    try {
        // Step 1: Calculate original size (the size of the loaded .comp file, which is the input)
        originalSize = compressedDataBytes.size();
        
        // Step 2: Call the backend utility for in-memory decompression.
        // This function handles the binary parsing of the QByteArray and returns the decompressed XML as std::string.
        std::string decompressedXmlStd = bpe_decompress_in_memory(compressedDataBytes);
        
        // Step 3: Convert the result back to QString for Qt display
        outputXML = QString::fromStdString(decompressedXmlStd);
        
        // Step 4: Calculate decompressed size (the size of the resulting XML output)
        decompressedSize = outputXML.toUtf8().size(); 
        
        // Step 5: Display the result in the output text area
        ui->outputTextEdit->setPlainText(outputXML);
        
        // Step 6: Update statistics and show the output sections
        updateStatistics();
        updateOutputVisibility(true);
        
        // Scroll to the top of the output text
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setValue(0);
        }
        
    } catch (const std::exception& e) {
        // Handle standard exceptions (e.g., parsing errors from the bridge logic)
        QMessageBox::critical(this, "Error", 
            QString("Failed to decompress XML: %1").arg(e.what()));
        updateOutputVisibility(false); // Hide output on failure
    } catch (...) {
        // Handle unknown exceptions
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while decompressing XML");
        updateOutputVisibility(false); // Hide output on failure
    }
}


void DecompressPage::updateStatistics()
{
    ui->originalSizeLabel->setText(QString::number(originalSize) + " bytes");
    ui->decompressedSizeLabel->setText(QString::number(decompressedSize) + " bytes"); 
    
    qint64 savedBytes = decompressedSize - originalSize; // Decompressed size is larger
    double ratio = originalSize > 0 ? 
        ((double)originalSize * 100.0 / decompressedSize) : 0.0;
    
    // Display compression ratio
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
    if (outputXML.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No decompressed XML to download!");
        return;
    }
    
    QString defaultFileName = "decompressed.xml"; 
    
    // Suggest a name based on the input file
    if (!currentFilePath.isEmpty()) {
        QFileInfo info(currentFilePath);
        // Change '.comp' extension to '.xml'
        defaultFileName = info.completeBaseName() + ".xml"; 
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Decompressed XML"),
        defaultFileName, 
        tr("XML Files (*.xml);;All Files (*)")
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

void DecompressPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
   
}