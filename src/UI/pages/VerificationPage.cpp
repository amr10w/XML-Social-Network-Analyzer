#include "VerificationPage.h"
#include "ui_VerificationPage.h"
#include "../../core/XMLValidator.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
QMessageBox::StandardButton showStyledMsg(QWidget* parent, 
                                          const QString& title, 
                                          const QString& text, 
                                          QMessageBox::Icon icon, 
                                          QMessageBox::StandardButtons buttons = QMessageBox::Ok)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(icon);
    msgBox.setStandardButtons(buttons);
    
    // THE GLOBAL STYLE FIX
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #2b2b2b; }"
        "QLabel { color: white; }"
        "QPushButton { "
        "  background-color: #404040; color: white; "
        "  border: 1px solid #666; padding: 5px 15px; border-radius: 3px;"
        "}"
        "QPushButton:hover { background-color: #505050; }"
        "QPushButton:pressed { background-color: #2d2d2d; }"
    );

    return (QMessageBox::StandardButton)msgBox.exec();
}

VerificationPage::VerificationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerificationPage)
{
    ui->setupUi(this);
}

VerificationPage::~VerificationPage()
{
    delete ui;
}

// ---------------------------------------------------------
// Navigation (Back Button)
// ---------------------------------------------------------
void VerificationPage::on_btnBack_clicked()
{
    // Emit the signal so the parent widget knows to change the view
    emit backClicked();
}

// ---------------------------------------------------------
// UI Logic: Browse and Load File
// ---------------------------------------------------------
void VerificationPage::on_btnBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open XML File"), "", tr("XML Files (*.xml);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file for reading."));
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Load content into the text area
    ui->txtInput->setPlainText(content);
    
    // Update label to show loaded path (optional)
    ui->lblInput->setText("Loaded: " + fileName);
}

void VerificationPage::on_btnVerify_clicked()
{
    QString qContent = ui->txtInput->toPlainText();
    if (qContent.trimmed().isEmpty()) {
        showStyledMsg(this, "Warning", "Please enter XML content or browse for a file first.", QMessageBox::Warning);
        return;
    }

    std::string content = qContent.toStdString();
    bool result = verify(content); 

    if (result) {
        showStyledMsg(this, "Success", "Verification succeeded.\nThe XML is valid.", QMessageBox::Information);
    } 
    else {
        ErrorInfo errors = countErrorSummary(content);
        
        QString errorMsg = QString("Verification failed.\nNumber of Errors: %1\n\n").arg(errors.count);
        for(size_t i = 0; i < errors.lines.size(); i++) {
            errorMsg += QString("Line %1: %2\n")
                        .arg(errors.lines[i])
                        .arg(QString::fromStdString(errors.descriptions[i]));
        }
        errorMsg += "\n\nDo you want to attempt to fix these errors automatically?";

        
        QMessageBox::StandardButton reply = showStyledMsg(this, 
                                                          "Errors Found", 
                                                          errorMsg, 
                                                          QMessageBox::Question, 
                                                          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            std::string fixedContent = fixXml(content, errors);
            ui->txtInput->setPlainText(QString::fromStdString(fixedContent));
            
            showStyledMsg(this, "Fixed", "The XML has been corrected. \nPlease review and save.", QMessageBox::Information);
        }
    }
}