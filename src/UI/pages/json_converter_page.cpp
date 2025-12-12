#include "json_converter_page.h"
#include "ui_json_converter_page.h"

#include "../../core/JsonConverter.h"
#include "../../core/FileUtils.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>

JsonConverterPage::JsonConverterPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonConverterPage)
{
    ui->setupUi(this);
    // setFixedSize(900, 750);

    // Initially hide output section
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
    
    ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    
    // Connect signals
    connect(ui->backButton, &QPushButton::clicked, this, &JsonConverterPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &JsonConverterPage::onBrowseFile);
    connect(ui->convertButton, &QPushButton::clicked, this, &JsonConverterPage::onConvertToJSON);
    connect(ui->downloadButton, &QPushButton::clicked, this, &JsonConverterPage::onDownload);
}

JsonConverterPage::~JsonConverterPage()
{
    delete ui;
}

void JsonConverterPage::onBackToOperations()
{
    // this->close(); 
    emit backToHomeClicked();
}

void JsonConverterPage::onBrowseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Browse XML File"), QString(), tr("XML Files (*.xml);;All Files (*)")
    );
    
    if (fileName.isEmpty()) return;
    
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
    
    // Hide output when new file is loaded to force re-conversion
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
}

void JsonConverterPage::onConvertToJSON()
{
    inputXML = ui->inputTextEdit->toPlainText();
    
    if (inputXML.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        // Convert
        std::string content = inputXML.toStdString();
        Tree<std::string>* root = buildTree(tokenizeXML(content));
        std::string jsonResult = JsonConverter::convert(root->getRoot());
        outputJSON = QString::fromStdString(jsonResult);

        // Display output
        ui->outputTextEdit->setPlainText(outputJSON);
        updateOutputVisibility();
        
        // Scroll to output
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) scrollBar->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Conversion Failed: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", "An unknown error occurred.");
    }
}

void JsonConverterPage::onDownload()
{
    if (outputJSON.isEmpty()) return;
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this, tr("Save JSON File"), "output.json", tr("JSON Files (*.json);;All Files (*)")
    );
    
    if (saveFileName.isEmpty()) return;
    
    QFile outFile(saveFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot save file!");
        return;
    }
    
    QTextStream out(&outFile);
    out << outputJSON;
    outFile.close();
}

void JsonConverterPage::updateOutputVisibility()
{
    bool hasOutput = !outputJSON.isEmpty();
    ui->outputLabel->setVisible(hasOutput);
    ui->outputTextEdit->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}