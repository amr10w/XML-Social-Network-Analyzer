#ifndef TEAMPAGE_H
#define TEAMPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>

namespace Ui {
class TeamPage;
}

struct TeamMember {
    QString name;
    QString id;
    QString role;
    QString imagePath;
};

class TeamPage : public QWidget
{
    Q_OBJECT

public:
    explicit TeamPage(QWidget *parent = nullptr);
    ~TeamPage();
    void addTeamMember(const TeamMember &member);
    void loadTeamMembers(const QList<TeamMember> &members);
    void clearTeamMembers();
    void loadTeamFromFile(const QString &filePath);

signals:
    // Signal when back button is clicked
    void backToWelcomeClicked();

private slots:
    // Slot for back button
    void on_btnBack_clicked();

private:
    Ui::TeamPage *ui;
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QGridLayout *gridLayout;
    int memberCount;
    QList<TeamMember> teamMembers;
    class QPushButton *backButton;
};

#endif // TEAMPAGE_H