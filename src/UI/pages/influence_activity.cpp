#include "influence_activity.h"
#include "ui_influence_activity_page.h"

#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <string>
#include "../../core/Graph.h" 

InfluenceAndActivity::InfluenceAndActivity(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InfluenceAndActivity)
{
    ui->setupUi(this);
    // setFixedSize(900, 750); // Ensuring full responsiveness

    // Signals are auto-connected by setupUi via on_widget_signal naming convention
}

InfluenceAndActivity::~InfluenceAndActivity()
{
    delete ui;
}

void InfluenceAndActivity::on_backButton_clicked()
{
    emit backToHomeClicked();
}

void InfluenceAndActivity::on_xmlTextEdit_textChanged()
{
    // If user modifies text, we can't rely on the file path anymore
    currentFilePath.clear();
}

void InfluenceAndActivity::on_mostInfluencerButton_clicked()
{
    if (ui->xmlTextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Input Missing", "Please load an XML file or paste XML content before searching for the Most Influencer.");
        return;
    }
    
    std::string content;
    if (!currentFilePath.isEmpty()) {
        QFile file(currentFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            content = in.readAll().toStdString();
        } else {
             content = ui->xmlTextEdit->toPlainText().toStdString();
        }
    } else {
        content = ui->xmlTextEdit->toPlainText().toStdString();
    }
    
    try {
        Graph graph(content);
        
        int mostInfluencerPersonId = graph.getMostInfluencerId();
        std::string mostInfluencerPersonName = graph.getName(mostInfluencerPersonId);
        
        QString resultMessage;
        if (mostInfluencerPersonId != -1) {
             resultMessage = QString("The Most Influencer:\nID: %1\nName: %2")
                                 .arg(mostInfluencerPersonId)
                                 .arg(QString::fromStdString(mostInfluencerPersonName));
        } else {
             resultMessage = "Could not identify the most influencer. Check the XML input format.";
        }
        
        QMessageBox resultBox(this);
        resultBox.setWindowTitle("Most Influencer User Result");
        resultBox.setText(resultMessage);
        resultBox.setIcon(QMessageBox::Information);
        resultBox.setStyleSheet("QLabel { color: black; } QPushButton { color: black; }"); 
        resultBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        resultBox.exec();
    
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Processing Error", QString("An error occurred during graph processing: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Processing Error", "An unknown error occurred during graph processing.");
    }
}

void InfluenceAndActivity::on_mostActiveButton_clicked()
{
    if (ui->xmlTextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Input Missing", "Please load an XML file or paste XML content before searching for the Most Active user.");
        return;
    }
    
    std::string content;
    if (!currentFilePath.isEmpty()) {
         QFile file(currentFilePath);
         if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             QTextStream in(&file);
             content = in.readAll().toStdString();
         } else {
             content = ui->xmlTextEdit->toPlainText().toStdString();
         }
    } else {
        content = ui->xmlTextEdit->toPlainText().toStdString();
    }
    
    try {
        Graph graph(content);
      
        int mostActivePersonId = graph.getMostActivePersonId();
        std::string mostActivePersonName = graph.getName(mostActivePersonId);
        
        QString resultMessage;
        if (mostActivePersonId != -1) { 
             resultMessage = QString("The Most Active User :\nID: %1\nName: %2")
                                 .arg(mostActivePersonId)
                                 .arg(QString::fromStdString(mostActivePersonName));
        } else {
             resultMessage = "Could not identify the most active user. Check the XML input format.";
        }
        
        QMessageBox resultBox(this);
        resultBox.setWindowTitle("Most Active User Result");
        resultBox.setText(resultMessage);
        resultBox.setIcon(QMessageBox::Information);
        resultBox.setStyleSheet("QLabel { color: black; } QPushButton { color: black; }"); 
        resultBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        resultBox.exec();
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Processing Error", QString("An error occurred during graph processing: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Processing Error", "An unknown error occurred during graph processing.");
    }
}

void InfluenceAndActivity::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Document", "", "XML Files (*.xml);;Text Files (*.txt);;All Files (*.*)"
    );

    if (!fileName.isEmpty()) {
        currentFilePath = fileName;
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            ui->xmlTextEdit->setText(in.readAll());
        }
    }
}