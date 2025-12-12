#include "formattingPage.h"
#include "ui_formattingPage.h"
#include "../../core/Tree.h" 
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>
#include "../../core/FileUtils.h"
#include <QScrollArea>
FormattingPage::FormattingPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormattingPage)
{
    ui->setupUi(this);
    // setFixedSize(900, 750);

    // Initially hide output section
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
    
    // Set placeholder
    ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    
    // Connect signals
    connect(ui->backButton, &QPushButton::clicked, this, &FormattingPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &FormattingPage::onBrowseFile);
    connect(ui->prettifyButton, &QPushButton::clicked, this, &FormattingPage::onPrettifyXML);
    connect(ui->downloadButton, &QPushButton::clicked, this, &FormattingPage::onDownload);
}

FormattingPage::~FormattingPage()
{
    delete ui;
}

void FormattingPage::onBackToOperations()
{
    // this->close();
    emit backToHomeClicked();
}

void FormattingPage::onBrowseFile()
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
    
    // Hide output when new file is loaded
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
    
}

void FormattingPage::onPrettifyXML()
{
    inputXML = ui->inputTextEdit->toPlainText();
    
    if (inputXML.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        std::string xmlContent = inputXML.toStdString();

        // Tokenize and build tree
        std::vector<Token> tokens = tokenizeXML(xmlContent);
        Tree<std::string>* tree = buildTree(tokens);

        // Generate prettified output
        tree->print_prettified();
        std::string prettified = tree->getPrettifyingString();

        outputXML = QString::fromStdString(prettified);

        // Display in output text edit
        ui->outputTextEdit->setPlainText(outputXML);
        
        // Show output section
        updateOutputVisibility();
        
        // Scroll to output
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setValue(0);
        }
        
        delete tree;
        
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to prettify XML: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while prettifying XML");
    }
}

void FormattingPage::onDownload()
{
    if (outputXML.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No formatted XML to download!");
        return;
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Prettified XML"),
        "output.xml",
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

void FormattingPage::updateOutputVisibility()
{
    bool hasOutput = !outputXML.isEmpty();
    ui->outputLabel->setVisible(hasOutput);
    ui->outputTextEdit->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}

void FormattingPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
}