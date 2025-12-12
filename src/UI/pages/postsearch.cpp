#include "postsearch.h"

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
// #include "../../core/SearchTopic.h" 
#include "../../core/SearchWord.h" 
#include "../../core/SearchTopic.h" 

// Theme colors
#define ORANGE_THEME "#FF6D1F"
#define BROWSE_BUTTON_COLOR "#FFA239"
#define LIGHTER_BUTTON_COLOR "#FF6D1F"

PostSearch::PostSearch(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(900, 750);
    setWindowTitle("Post Search");

    // Global styling
    setStyleSheet(
        "QWidget { background-color: #f0f0f0; }"
        "QPushButton { font-weight: bold; border-radius: 10px; padding: 10px; }"
        
    );

    setupUI();

    connect(postsearchByTopics, &QPushButton::clicked, this, &PostSearch::on_searchByTopics_clicked);
    connect(postsearchByWords, &QPushButton::clicked, this, &PostSearch::on_searchByWords_clicked);
    connect(browseButton, &QPushButton::clicked, this, &PostSearch::on_browseButton_clicked);
}

// ----------------- UI SETUP -----------------

QPushButton* PostSearch::createStyledButton(const QString& text, const QString& style)
{
    QPushButton *button = new QPushButton(text);
    button->setStyleSheet(style);
    return button;
}

void PostSearch::setupUI()
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
    QLabel *iconTitle = new QLabel("Post Search");
    iconTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    headerLayout->addWidget(iconTitle);

    QLabel *description = new QLabel("Finding posts related to topics or words");
    description->setStyleSheet("font-size: 14px;");
    headerLayout->addWidget(description);

    // Back Button (Added)
    QPushButton *backBtn = new QPushButton("Back");
    backBtn->setStyleSheet("background-color: transparent; border: 1px solid white; color: white; padding: 5px; border-radius: 5px;");
    connect(backBtn, &QPushButton::clicked, this, &PostSearch::on_backButton_clicked);
    headerLayout->addWidget(backBtn);
    
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
        " background-color:#FFECC0 ; "
        " color: #FF6D1F; "
        " border: 1px solid #FFECC0 ; "
        "} "
        "QPushButton:hover { background-color: #FFCF71 ; }"
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
    "   background: #EEEEEE ;" /* Use 'background' instead of 'background-color' */
    "   color: black;"    
    "   margin-bottom:10px;"      
    "   border: 20px solid black;"
    "   padding: 10px;"
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

    postsearchByTopics = createStyledButton("Post search by topics", buttonStyle);
    postsearchByTopics->setMinimumHeight(50);

    postsearchByWords = createStyledButton("Post search by words", buttonStyle);
    postsearchByWords->setMinimumHeight(50);

    actionLayout->addWidget(postsearchByTopics);
    actionLayout->addWidget(postsearchByWords);

    inputLayout->addLayout(actionLayout); // Add the horizontal button layout to the vertical input layout

    // Add the single combined container to the main window layout
    mainLayout->addWidget(inputContainer);

    mainLayout->addStretch(1);
}

// ----------------- SLOTS -----------------

void PostSearch::on_searchByTopics_clicked()
{
    // 1. Get user input (Topics)
    QInputDialog dialog(this);

    // 2. Set the window title and label text (matching the original call)
    dialog.setWindowTitle("Search by Topics");
    dialog.setLabelText("Enter topics:");
    dialog.setTextEchoMode(QLineEdit::Normal);
    
    // 3. APPLY STYLING: Set the stylesheet to target the QLineEdit inside the dialog
    dialog.setStyleSheet(
        "QLineEdit {"
        "  color: black;"  // <--- SETS THE COLOR OF THE TEXT THE USER TYPES
        "  font-weight: bold;" // Optional: Make the text bold
        "}"
        // Optional: You can style the whole dialog window too
        "QInputDialog {"
        "  background-color: #ECECEC;" 
        "}"
        "QLabel {"
        "  color: black;" // Forces the prompt text color to be black
        "  font-weight: bold;" // Optional: Makes the prompt stand out
        "}"
    );

    // 4. Execute the dialog and retrieve the result
    bool ok;
    QString input;
    if (dialog.exec() == QDialog::Accepted) {
        ok = true;
        input = dialog.textValue();
    } else {
        ok = false;
    }
    
    // 5. Use the result just as before
    if (!ok || input.isEmpty()) { 
        return; 
    }
    // 2. Get XML Content from QTextEdit
    QString xmlContentQt = xmlTextEdit->toPlainText();

    if (xmlContentQt.isEmpty()) {
        QMessageBox::warning(this, "Input Missing", "Please load or paste XML content before searching.");
        return;
    }

    // Convert QString to std::string for the core C++ function
    std::string xmlContentStd = xmlContentQt.toStdString();
    std::string topicStd = input.toStdString();

    // --- Core Search Logic ---
    try {
        // 3. Call the external function, returning PostMatchTopic vector
        std::vector<PostMatch> results = searchPostsByTopic(xmlContentStd, topicStd);

        // 4. Format the Results for Display (Manual QString building)
        QString resultText;
        if (results.empty()) {
            resultText = QString("No posts found containing the topic: '%1'.").arg(input);
        } else {
            resultText = QString("✅ Found %1 matching posts containing topic: '%2'\n\n--- RESULTS ---\n").arg(results.size()).arg(input);
            
            // The PostMatchTopic struct (id and text fields) is used here:
            for (size_t i = 0; i < results.size(); ++i) {
    // 1. Get the ID as std::string
    std::string postIdStd = results[i].id;
    
    // 2. Determine the display string based on the value
    QString postIdDisplay;
    if (postIdStd.empty()) {
        postIdDisplay = "(unknown)";
    } else {
        postIdDisplay = QString::fromStdString(postIdStd);
    }
    
    // Formatting each match for display
    // 3. Append the formatted ID to the result text, using the conditional value
    resultText += QString("UserID: %1\n")
        .arg(postIdDisplay);
    
    // Show a snippet of the text (e.g., first 100 characters)
    QString postText = QString::fromStdString(results[i].text).simplified();
    resultText += QString("Text: %1...\n\n")
        .arg(postText.left(100)); // Limits output to prevent massive message box
}
        }

        // 5. Display the Results in a new window (QMessageBox)
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🔍 Search Results - Topics");
        msgBox.setText(resultText);
        // Allows the user to select and copy the results
        msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        msgBox.setMinimumWidth(500);

        // Styling (Using your defined styles)
         msgBox.setStyleSheet(
            // Style the entire dialog window/container
            "QMessageBox {"
            // "  background-color: #f7f7f7; " // Light gray background
            "  font-family: Arial; "
            "text-align: center;"
            "}"
            // Style the label containing the actual message text
            "QLabel {"
            "  color: #333333; " // Dark text color
            "  font-size: 14px; "
            "  padding: 10px; "
            "}"
            // Style the standard buttons (like OK)
            "QPushButton {"
            "  background-color: #FF6D1F; " // Orange theme color
            "  color: white; "
            "  border: 1px solid #FF6D1F; "
            "  border-radius: 5px; "
            "  padding: 10px 10px; "
            "}"
            "QPushButton:hover {"
            "  background-color: #FF8844; " // Lighter orange on hover
            "}"
        );
        msgBox.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "🚨 Search Error", QString("An internal error occurred: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "🚨 Search Error", "An unknown error occurred during search execution.");
    }
}



void PostSearch::on_searchByWords_clicked()
{
    // 1. Get user input (Word)
   QInputDialog dialog(this);

    // 2. Set the window title and label text (matching the original call)
    dialog.setWindowTitle("Search by Words");
    dialog.setLabelText("Enter specific word:");
    dialog.setTextEchoMode(QLineEdit::Normal);
    
    // 3. APPLY STYLING: Set the stylesheet to target the QLineEdit inside the dialog
    dialog.setStyleSheet(
        "QLineEdit {"
        "  color: black;"  // <--- SETS THE COLOR OF THE TEXT THE USER TYPES
        "  font-weight: bold;" // Optional: Make the text bold
        "}"
        // Optional: You can style the whole dialog window too
        "QInputDialog {"
        "  background-color: #ECECEC;" 
        "}"
        "QLabel {"
        "  color: black;" // Forces the prompt text color to be black
        "  font-weight: bold;" // Optional: Makes the prompt stand out
        "}"
    );

    // 4. Execute the dialog and retrieve the result
    bool ok;
    QString input;
    if (dialog.exec() == QDialog::Accepted) {
        ok = true;
        input = dialog.textValue();
    } else {
        ok = false;
    }
    
    // 5. Use the result just as before
    if (!ok || input.isEmpty()) { 
        return; 
    }
    // 2. Get XML Content from QTextEdit
    QString xmlContentQt = xmlTextEdit->toPlainText();

    if (xmlContentQt.isEmpty())
    {
        QMessageBox::warning(this, "Input Missing", "Please load or paste XML content before searching.");
        return;
    }

    // Convert QString to std::string for the core C++ function
    std::string xmlContentStd = xmlContentQt.toStdString();
    std::string wordStd = input.toStdString();

    // --- Core Search Logic ---
    try {
        // 3. Call the external function from SearchWord.h
        // Signature: std::vector<PostMatch> searchPostsByWord(const std::string& xmlContent, const std::string& word);
             std::vector<PostMatch> results = searchPostsByWord(xmlContentStd,wordStd);

        // 4. Format the Results for Display
        QString resultText;
        if (results.empty()) {
            resultText = QString("No posts found containing the word: '%1'.").arg(input);
        } else {
            resultText = QString("✅ Found %1 matching posts containing word: '%2'\n\n--- RESULTS ---\n").arg(results.size()).arg(input);
            
            // The PostMatch struct you provided has id and text fields:
            // struct PostMatch { std::string id; std::string text; };
            for (size_t i = 0; i < results.size(); ++i) {
             // Formatting each match for display
    
            // 1. Get the ID as std::string
            std::string postIdStd = results[i].id;
            
            // 2. Determine the display string based on the value
            QString postIdDisplay;
            if (postIdStd.empty()) {
                postIdDisplay = "(unknown)";
            } else {
                postIdDisplay = QString::fromStdString(postIdStd);
            }
            
            // 3. Append the formatted ID to the result text, using the conditional value
            resultText += QString("UserID: %1\n")
                .arg(postIdDisplay);
            
            // Show a snippet of the text (e.g., first 100 characters)
            QString postText = QString::fromStdString(results[i].text).simplified();
            resultText += QString("Text: %1...\n\n")
                .arg(postText.left(100)); // Limits output to prevent massive message box
        }
                }

        // 5. Display the Results in a new window (QMessageBox)
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🔍 Search Results - Words");
        msgBox.setText(resultText);
        // Allows the user to select and copy the results
        msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        msgBox.setMinimumWidth(500);


        msgBox.setStyleSheet(
            // Style the entire dialog window/container
            "QMessageBox {"
            // "  background-color: #f7f7f7; " // Light gray background
            "  font-family: Arial; "
            "text-align: center;"
            "}"
            // Style the label containing the actual message text
            "QLabel {"
            "  color: #333333; " // Dark text color
            "  font-size: 14px; "
            "  padding: 10px; "
            "}"
            // Style the standard buttons (like OK)
            "QPushButton {"
            "  background-color: #FF6D1F; " // Orange theme color
            "  color: white; "
            "  border: 1px solid #FF6D1F; "
            "  border-radius: 5px; "
            "  padding: 10px 10px; "
            "}"
            "QPushButton:hover {"
            "  background-color: #FF8844; " // Lighter orange on hover
            "}"
        );
        msgBox.exec();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "🚨 Search Error", QString("An internal error occurred: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "🚨 Search Error", "An unknown error occurred during search execution.");
    }
}





void PostSearch::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Document", "", "Text Files (*.txt);;XML Files (*.xml);;All Files (*.*)"
    );

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            xmlTextEdit->setText(in.readAll());
            // QMessageBox::information(this, "File Loaded", "Loaded: " + fileName);
        }
    }
}

void PostSearch::on_backButton_clicked()
{
    emit backClicked();
}
