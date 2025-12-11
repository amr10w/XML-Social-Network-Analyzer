#include "teampage.h"
#include "ui_teampage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>

TeamPage::TeamPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeamPage),
    memberCount(0),
    backButton(nullptr)
{
    ui->setupUi(this);
    
    // Get back button from UI and connect it
    backButton = ui->backButton;
    if (backButton) {
        connect(backButton, &QPushButton::clicked, this, &TeamPage::on_btnBack_clicked);
    }
    
    // Connect back button signal
    connect(backButton, &QPushButton::clicked, this, &TeamPage::on_btnBack_clicked);
    
    // Create scroll area for team members
    scrollArea = findChild<QScrollArea*>("scrollArea");
    if (scrollArea) {
        // Create outer container with margins
        QWidget *outerContainer = new QWidget();
        QVBoxLayout *outerLayout = new QVBoxLayout(outerContainer);
        outerLayout->setContentsMargins(0, 20, 0, 20);
        
        // Create centered container widget
        containerWidget = new QWidget();
        containerWidget->setStyleSheet("background-color: white; border-radius: 10px; padding: 15px;");
        containerWidget->setMaximumWidth(900);
        
        gridLayout = new QGridLayout(containerWidget);
        gridLayout->setSpacing(15);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        
        // Add container to outer layout with stretch
        outerLayout->addStretch();
        outerLayout->addWidget(containerWidget, 0, Qt::AlignCenter);
        outerLayout->addStretch();
        
        scrollArea->setWidget(outerContainer);
    }
    
    // Load team members from file
    loadTeamFromFile("./data/team_members.txt");
}

TeamPage::~TeamPage()
{
    delete ui;
}

void TeamPage::addTeamMember(const TeamMember &member)
{
    if (!gridLayout) return;
    
    teamMembers.append(member);
    
    // Create container for team member card
    QWidget *memberCard = new QWidget();
    QVBoxLayout *cardLayout = new QVBoxLayout(memberCard);
    cardLayout->setContentsMargins(5, 5, 5, 5);
    cardLayout->setSpacing(2);
    
    // Member image in circular frame
    QLabel *imageLabel = new QLabel();
    QPixmap pixmap(member.imagePath);
    
    if (!pixmap.isNull()) {
        // Scale and create circular pixmap
        pixmap = pixmap.scaledToWidth(100, Qt::SmoothTransformation);
        QPixmap circularPixmap(100, 100);
        circularPixmap.fill(Qt::white);
        
        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 99, 99);
        
        // Create a circular mask
        QPainterPath path;
        path.addEllipse(0, 0, 100, 100);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pixmap);
        painter.end();
        
        imageLabel->setPixmap(circularPixmap);
    } else {
        // Create circular placeholder with user icon style
        QPixmap circlePlaceholder(100, 100);
        circlePlaceholder.fill(Qt::white);
        
        QPainter painter(&circlePlaceholder);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor("#E0E0E0"));
        painter.setPen(QPen(QColor("#D0D0D0"), 1));
        painter.drawEllipse(0, 0, 99, 99);
        
        // Draw simple user icon (two circles for head and body)
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#999999"));
        painter.drawEllipse(35, 15, 30, 30); // Head
        painter.drawEllipse(25, 50, 50, 35); // Body
        painter.end();
        
        imageLabel->setPixmap(circlePlaceholder);
    }
    
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setFixedSize(100, 100);
    
    // Create a container widget to center the image
    QWidget *imageContainer = new QWidget();
    QHBoxLayout *imageLayout = new QHBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);
    imageLayout->addStretch();
    imageLayout->addWidget(imageLabel);
    imageLayout->addStretch();
    
    // Member name
    QLabel *nameLabel = new QLabel(member.name);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333; font-family: 'Segoe UI', 'Arial', sans-serif; letter-spacing: 0.5px;");
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setWordWrap(true);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    nameLabel->setMinimumHeight(35);
    
    // Member ID
    QLabel *idLabel = new QLabel(member.id);
    idLabel->setStyleSheet("font-size: 12px; color: #666; padding: 3px; border-radius: 3px; font-family: 'Courier New', monospace;");
    idLabel->setAlignment(Qt::AlignCenter);
    idLabel->setMinimumHeight(25);
    
    // Member role
    QLabel *roleLabel = new QLabel(member.role);
    roleLabel->setStyleSheet("font-size: 11px; color: #222; padding: 5px; border-radius: 3px; font-weight: bold; font-style: italic;");
    roleLabel->setAlignment(Qt::AlignCenter);
    roleLabel->setMinimumHeight(25);
    
    // Add to card layout
    cardLayout->addWidget(imageContainer);
    cardLayout->addWidget(nameLabel);
    cardLayout->addWidget(idLabel);
    cardLayout->addWidget(roleLabel);
    
    // Set card style with shadow effect
    memberCard->setStyleSheet("QWidget { border: none; border-radius: 8px; background-color: #f5f5f5; } QWidget:hover { background-color: #e8e8e8; }");
    memberCard->setMaximumWidth(200);
    
    // Add to grid (3 columns)
    int row = memberCount / 3;
    int col = memberCount % 3;
    gridLayout->addWidget(memberCard, row, col);
    
    memberCount++;
}

void TeamPage::loadTeamMembers(const QList<TeamMember> &members)
{
    clearTeamMembers();
    for (const TeamMember &member : members) {
        addTeamMember(member);
    }
}

void TeamPage::clearTeamMembers()
{
    if (gridLayout) {
        QLayoutItem *item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    memberCount = 0;
    teamMembers.clear();
}

void TeamPage::loadTeamFromFile(const QString &filePath)
{
    QFile file(filePath);
    
    // Try absolute path if relative fails
    if (!file.exists()) {
        QString absolutePath = QCoreApplication::applicationDirPath() + "/../data/team_members.txt";
        file.setFileName(absolutePath);
    }
    
    qDebug() << "Loading team from:" << file.fileName();
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.fileName();
        return;
    }
    
    QTextStream in(&file);
    QList<TeamMember> members;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(",");
        if (parts.size() >= 3) {
            TeamMember member;
            member.name = parts[0];
            member.id = parts[1];
            member.role = parts[2];
            member.imagePath = parts.size() > 3 ? parts[3] : "";
            members.append(member);
            qDebug() << "Loaded member:" << member.name;
        }
    }
    
    file.close();
    qDebug() << "Total members loaded:" << members.size();
    loadTeamMembers(members);
}

void TeamPage::on_btnBack_clicked()
{
    emit backToWelcomeClicked();
}