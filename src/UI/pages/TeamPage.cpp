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
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEvent>
#include <QDesktopServices>
#include <QUrl>

// Helper class for the hoverable card
class TeamCard : public QWidget {
public:
    TeamCard(QWidget* parent = nullptr) : QWidget(parent) {
        // Initial Style
        setStyleSheet(
            "QWidget { "
            "  background-color: white; "
            "  border-radius: 20px; "
            "  border: 1px solid #EFEFEF; "
            "}"
        );
        
        // Shadow Effect
        shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 15)); // Soft initial shadow
        setGraphicsEffect(shadow);
    }

protected:
    void enterEvent(QEnterEvent* event) override {
        // Enlarge shadow and move up slightly
        QPropertyAnimation* anim = new QPropertyAnimation(shadow, "blurRadius");
        anim->setDuration(200);
        anim->setStartValue(shadow->blurRadius());
        anim->setEndValue(40);
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* animOffset = new QPropertyAnimation(shadow, "offset");
        animOffset->setDuration(200);
        animOffset->setStartValue(shadow->offset());
        animOffset->setEndValue(QPointF(0, 8));
        animOffset->start(QAbstractAnimation::DeleteWhenStopped);

        // Ideally we would animate geometry/pos here for translateY, 
        // but simple shadow pop is safer for grid layout constraints.
        // We can mimic highlighting:
        setStyleSheet(
            "QWidget { "
            "  background-color: white; "
            "  border-radius: 20px; "
            "  border: 1px solid #4318FF; " // Highlight border on hover
            "}"
        );
        
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        // Restore shadow
        QPropertyAnimation* anim = new QPropertyAnimation(shadow, "blurRadius");
        anim->setDuration(200);
        anim->setStartValue(shadow->blurRadius());
        anim->setEndValue(20);
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* animOffset = new QPropertyAnimation(shadow, "offset");
        animOffset->setDuration(200);
        animOffset->setStartValue(shadow->offset());
        animOffset->setEndValue(QPointF(0, 4));
        animOffset->start(QAbstractAnimation::DeleteWhenStopped);

        setStyleSheet(
            "QWidget { "
            "  background-color: white; "
            "  border-radius: 20px; "
            "  border: 1px solid #EFEFEF; "
            "}"
        );

        QWidget::leaveEvent(event);
    }

private:
    QGraphicsDropShadowEffect* shadow;
};


TeamPage::TeamPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeamPage),
    memberCount(0),
    backButton(nullptr)
{
    ui->setupUi(this);
    
    // Connect signals
    connect(ui->backButton, &QPushButton::clicked, this, &TeamPage::on_btnBack_clicked);
    
    // Setup Grid
    scrollArea = findChild<QScrollArea*>("scrollArea");
    if (scrollArea) {
        // Clean container
        QWidget *outerContainer = new QWidget();
        // outerContainer->setStyleSheet("background: transparent;");
        QVBoxLayout *outerLayout = new QVBoxLayout(outerContainer);
        outerLayout->setContentsMargins(0, 10, 0, 20);
        
        // Grid Container
        containerWidget = new QWidget();
        containerWidget->setStyleSheet("background: transparent;");
        
        gridLayout = new QGridLayout(containerWidget);
        gridLayout->setSpacing(30); // More spacing for breathing room
        gridLayout->setContentsMargins(10, 10, 10, 10);
        
        // Centering logic
        outerLayout->addWidget(containerWidget, 0, Qt::AlignTop | Qt::AlignHCenter); 
        
        scrollArea->setWidget(outerContainer);
    }
    
    loadTeamFromFile(":/new/Prefix1/files/team_members.txt");
}

TeamPage::~TeamPage()
{
    delete ui;
}

void TeamPage::addTeamMember(const TeamMember &member)
{
    if (!gridLayout) return;

    teamMembers.append(member);
    
    // Create custom hoverable card
    TeamCard *memberCard = new TeamCard();
    memberCard->setFixedSize(260, 360); // Increased height for link
    
    QVBoxLayout *cardLayout = new QVBoxLayout(memberCard);
    cardLayout->setContentsMargins(20, 30, 20, 30);
    cardLayout->setSpacing(10);
    
    // --- 1. Avatar (Circular) ---
    QLabel *imageLabel = new QLabel();
    QPixmap pixmap(member.imagePath);
    
    // Create a circular avatar base
    int avatarSize = 110;
    QPixmap circularPixmap(avatarSize, avatarSize);
    circularPixmap.fill(Qt::transparent);
    
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background circle (accent color for specific roles or just generic cool gradient?)
    // Let's use a subtle gradient border ring
    QLinearGradient borderGrad(0, 0, avatarSize, avatarSize);
    borderGrad.setColorAt(0, QColor("#4318FF")); // Indigo
    borderGrad.setColorAt(1, QColor("#00D1FF")); // Cyan
    QPen borderPen(borderGrad, 4);
    painter.setPen(borderPen);
    painter.setBrush(Qt::white);
    painter.drawEllipse(2, 2, avatarSize-4, avatarSize-4);

    // Draw the actual image clipped
    QPainterPath path;
    path.addEllipse(5, 5, avatarSize-10, avatarSize-10);
    painter.setClipPath(path);
    
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(avatarSize, avatarSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        painter.drawPixmap(5, 5, pixmap);
    } else {
        // Fallback Icon
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#E0E5F2")); // Light gray
        painter.drawRect(0, 0, avatarSize, avatarSize);
        // Simple head/body
        painter.setBrush(QColor("#A3AED0"));
        painter.drawEllipse(35, 25, 40, 40);
        painter.drawEllipse(25, 70, 60, 50);
    }
    painter.end();
    
    imageLabel->setPixmap(circularPixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(imageLabel);
    
    cardLayout->addSpacing(10);

    // --- 2. Name & ID ---
    QLabel *nameLabel = new QLabel(member.name);
    nameLabel->setStyleSheet(
        "color: #1B2559; " // Dark Navy
        "font-size: 18px; "
        "font-weight: 800; "
        "font-family: 'Segoe UI', sans-serif; "
        "background: transparent; border: none;"
    );
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setWordWrap(true);
    cardLayout->addWidget(nameLabel);

    QLabel *idLabel = new QLabel(member.id);
    idLabel->setStyleSheet(
        "color: #A3AED0; " // Soft Gray
        "font-size: 13px; "
        "background: transparent; border: none; font-family: monospace;"
    );
    idLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(idLabel);
    
    // --- 3. Role Badge ---
    // Make the role look like a pill badge
    QLabel *roleLabel = new QLabel(member.role);
    roleLabel->setAlignment(Qt::AlignCenter);
    
    // Dynamic color based on role?
    QString roleBg = "#F4F7FE";
    QString roleColor = "#4318FF";
    
    if (member.role.contains("Lead") || member.role.contains("Manager")) {
        roleBg = "#E8EAF6"; // Indigo Tint
        roleColor = "#303F9F";
    } else {
        roleBg = "#E0F2F1"; // Teal Tint
        roleColor = "#00695C";
    }

    roleLabel->setStyleSheet(QString(
        "QLabel { "
        "  background-color: %1; "
        "  color: %2; "
        "  padding: 6px 15px; "
        "  border-radius: 12px; "
        "  font-weight: 700; "
        "  font-size: 12px; "
        "}"
    ).arg(roleBg).arg(roleColor));
    
    // Container for centering the badge
    QHBoxLayout *badgeLayout = new QHBoxLayout();
    badgeLayout->addStretch();
    badgeLayout->addWidget(roleLabel);
    badgeLayout->addStretch();
    
    cardLayout->addLayout(badgeLayout);

    // --- 4. LinkedIn Link (Redesigned as Button) ---
    if (!member.linkedinUrl.isEmpty()) {
        QPushButton *linkedinBtn = new QPushButton("LinkedIn Profile");
        linkedinBtn->setCursor(Qt::PointingHandCursor);
        linkedinBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #0077b5;" // LinkedIn Blue
            "  color: white;"
            "  border: none;"
            "  border-radius: 6px;"
            "  padding: 6px 12px;"
            "  font-weight: bold;"
            "  font-family: 'Segoe UI', sans-serif;"
            "}"
            "QPushButton:hover {"
            "  background-color: #006097;" // Slightly darker on hover
            "}"
        );
        
        connect(linkedinBtn, &QPushButton::clicked, [member](){
            QDesktopServices::openUrl(QUrl(member.linkedinUrl));
        });
        
        // Center the button
        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addStretch();
        btnLayout->addWidget(linkedinBtn);
        btnLayout->addStretch();
        
        cardLayout->addSpacing(10);
        cardLayout->addLayout(btnLayout);
    } else {
         // Optional spacer if no button
         // cardLayout->addSpacing(30);
    }
    
    cardLayout->addStretch();
    
    // Add to grid logic
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
    if (!file.exists()) {
        QString absolutePath = QCoreApplication::applicationDirPath() + ":/new/Prefix1/files/team_members.txt";
        file.setFileName(absolutePath);
    }
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
            member.name = parts[0].trimmed();
            member.id = parts[1].trimmed();
            member.role = parts[2].trimmed();
            // Check for Image Path (4th column)
            if (parts.size() >= 4) {
                 member.imagePath = parts[3].trimmed();
            }
            // Check for LinkedIn URL (5th column)
            if (parts.size() >= 5) {
                 member.linkedinUrl = parts[4].trimmed();
            }
            
            members.append(member);
        }
    }
    
    file.close();
    loadTeamMembers(members);
}

void TeamPage::on_btnBack_clicked()
{
    emit backToWelcomeClicked();
}