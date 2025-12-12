#include "influence_activity.h" // Includes the corrected header file

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QPixmap>
#include <string> // Required for std::string conversion
#include "../../core/Graph.h" 


// Theme colors
#define ORANGE_THEME "#19183B"
#define BROWSE_BUTTON_COLOR "#19183B"
#define LIGHTER_BUTTON_COLOR "#000000"

// Constructor change: PostSearch::PostSearch -> InfluenceAndActivity::InfluenceAndActivity
InfluenceAndActivity::InfluenceAndActivity(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(900, 750);
    setWindowTitle("Influency and Activity");

    // Global styling
    setStyleSheet(
        "QWidget { background-color: #f0f0f0; }"
        "QPushButton { font-weight: bold; border-radius: 10px; padding: 10px; }"
        
    );

    setupUI();

    // Connects use the new function names
    connect(mostinfluencer, &QPushButton::clicked, this, &InfluenceAndActivity::on_mostInfluencer_clicked); 
    connect(mostactive, &QPushButton::clicked, this, &InfluenceAndActivity::on_mostActive_clicked);
    connect(browseButton, &QPushButton::clicked, this, &InfluenceAndActivity::on_browseButton_clicked);
}

// ----------------- UI SETUP -----------------

// Method signature change: PostSearch::createStyledButton -> InfluenceAndActivity::createStyledButton
QPushButton* InfluenceAndActivity::createStyledButton(const QString& text, const QString& style)
{
    QPushButton *button = new QPushButton(text);
    button->setStyleSheet(style);
    return button;
}

// Method signature change: PostSearch::setupUI -> InfluenceAndActivity::setupUI
void InfluenceAndActivity::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    // Removed mainLayout contents margins to use margins on inner containers
    mainLayout->setContentsMargins(20, 30, 20, 30);

    // Header (No change)
    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget { background-color: " BROWSE_BUTTON_COLOR "; color: white; padding: 10px; border-radius:10px}"
        "QLabel { background-color: transparent; }"
    );

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    // Header title and description updated
    QLabel *iconTitle = new QLabel("Influency and Activity"); 
    iconTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    headerLayout->addWidget(iconTitle);

    QLabel *description = new QLabel("Finding the most influencer and the most active user"); 
    description->setStyleSheet("font-size: 14px;");
    headerLayout->addWidget(description);

    mainLayout->addWidget(headerWidget);
    mainLayout->addSpacing(10); // Small space before the main content container

    // Input & Action Container (Combined, with white background)
    QWidget *inputContainer = new QWidget();
    inputContainer->setStyleSheet(
        "QWidget { background-color: white; border-radius: 10px; }" // White background applied here
    );

    QVBoxLayout *inputLayout = new QVBoxLayout(inputContainer);
    // Adjusted margins for padding inside the white container
    inputLayout->setContentsMargins(30, 20, 30, 30);
    inputLayout->setSpacing(15); // Add spacing between elements

    // --- Input Elements ---

    QLabel *inputLabel = new QLabel("Input:XML File");
    inputLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 5px; color:black");
    inputLayout->addWidget(inputLabel);

   browseButton = createStyledButton(
    "Browse XML File",
    "QPushButton { "
    " background-color: #19183B; "
    " color: white; "
    " border: 1px solid #FFECC0; "
    "} "
    "QPushButton:hover { background-color: black; }"
);
    browseButton->setMinimumHeight(50);
    inputLayout->addWidget(browseButton);

    QLabel *orLabel = new QLabel("OR");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setStyleSheet(" color:black");
    inputLayout->addWidget(orLabel);

    xmlTextEdit = new QTextEdit();
    xmlTextEdit->setPlaceholderText("Paste your XML content here...");
    xmlTextEdit->setMinimumHeight(190);
    xmlTextEdit->setStyleSheet(
    "QTextEdit {"
    " background: #EEEEEE ;" /* Use 'background' instead of 'background-color' */
    " color: black;"    
    " margin-bottom:10px;"      
    " border: 20px solid black;"
    " padding: 10px;"
    "}"
);

    inputLayout->addWidget(xmlTextEdit);

    inputLayout->addSpacing(25); // Spacer before action buttons

    // --- Action Buttons (Now inside the same white container) ---

    QHBoxLayout *actionLayout = new QHBoxLayout(); // Use QHBoxLayout for horizontal buttons
    actionLayout->setSpacing(10); // Spacing between buttons

    QString buttonStyle =
        "QPushButton { background-color: " BROWSE_BUTTON_COLOR "; color: white; font-size: 16px; padding: 15px; } "
        "QPushButton:hover { background-color: " LIGHTER_BUTTON_COLOR " }";

    // Button text updated
    mostinfluencer = createStyledButton("Most Influencer", buttonStyle); 
    mostinfluencer->setMinimumHeight(50);

    // Button text updated
    mostactive = createStyledButton("Most Active", buttonStyle); 
    mostactive->setMinimumHeight(50);

    actionLayout->addWidget(mostinfluencer);
    actionLayout->addWidget(mostactive);

    inputLayout->addLayout(actionLayout); // Add the horizontal button layout to the vertical input layout

    // Add the single combined container to the main window layout
    mainLayout->addWidget(inputContainer);

    mainLayout->addStretch(1);
}

// ----------------- SLOTS -----------------

void InfluenceAndActivity::on_mostInfluencer_clicked()
{
    if (xmlTextEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Missing"); 
        msgBox.setText("Please load an XML file or paste XML content before searching for the Most Influencer.");
        msgBox.setIcon(QMessageBox::Warning); 
        
        msgBox.setStyleSheet("QLabel { color: black; }"); 
        
        msgBox.exec();
        return;
    }
    
    
    QString xmlContentQ = xmlTextEdit->toPlainText();
    std::string content = xmlContentQ.toStdString();
    
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
        resultBox.setWindowTitle("Most influencer User Result");
        resultBox.setText(resultMessage);
        resultBox.setIcon(QMessageBox::NoIcon);
        
       

        resultBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
       


    resultBox.setStyleSheet(
            "QMessageBox {"
            "  font-family: Arial; "
            "text-align: center;"
           
            "}"
            "QLabel {"
            "  color: #333333; " 
            "  font-size: 20px; "
            "  padding: 10px; "
            "}"
            "QPushButton {"
            "  background-color: #19183B; " 
            "  color: white; "
            "  border-radius: 5px; "
                        "  font-size: 15px; "

            "  padding: 15px 15px; "
            "  margin-bottom: 5px ; "

            "}"
            "QPushButton:hover {"
            "  background-color: black; " 
            "}"
        );
        
resultBox.layout()->setContentsMargins(15, 25, 20, 5);  // enlarge inside
resultBox.layout()->setSpacing(10);
resultBox.exec();
    
        
    } catch (const std::exception& e) {
        // Handle potential exceptions during Graph processing
        QMessageBox::critical(this, "Processing Error", QString("An error occurred during graph processing: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Processing Error", "An unknown error occurred during graph processing.");
    }

    
}



void InfluenceAndActivity::on_mostActive_clicked()
{
    
    if (xmlTextEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Missing"); 
        msgBox.setText("Please load an XML file or paste XML content before searching for the Most Active user.");
        
        msgBox.setIcon(QMessageBox::Warning);
        
        
        
        msgBox.setStyleSheet("QLabel { color: black; }"); 
        
        msgBox.exec();
        return;
    }
    
    
    
    
    QString xmlContentQ = xmlTextEdit->toPlainText();
    std::string content = xmlContentQ.toStdString();
    
    
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
        resultBox.setIcon(QMessageBox::NoIcon);
     

        resultBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        


    resultBox.setStyleSheet(
            
            "QMessageBox {"
            "  font-family: Arial; "
            "text-align: center;"
           
            "}"
            
            "QLabel {"
            "  color: #333333; " 
            "  font-size: 20px; "
            "  padding: 10px; "
            "}"
            "QPushButton {"
            "  background-color: #19183B; " 
            "  color: white; "
            "  border-radius: 5px; "
                        "  font-size: 15px; "

            "  padding: 15px 15px; "
            "  margin-bottom: 5px ; "

            "}"
            "QPushButton:hover {"
            "  background-color: black; " 
            "}"
        );
        
resultBox.layout()->setContentsMargins(15, 25, 20, 5);  // enlarge inside
resultBox.layout()->setSpacing(10);
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
        this, "Open Document", "", "Text Files (*.txt);;XML Files (*.xml);;All Files (*.*)"
    );

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            xmlTextEdit->setText(in.readAll());
        }
    }
}