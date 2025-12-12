// influence_activity.h

#ifndef INFLUENCE_ACTIVITY_H
#define INFLUENCE_ACTIVITY_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

// Forward declarations for external core functions/structs (assuming they are needed)
// ...

// Class name change: PostSearch -> InfluenceAndActivity
class InfluenceAndActivity : public QMainWindow 
{
    Q_OBJECT

public:
    // Constructor name must match the class name
    explicit InfluenceAndActivity(QWidget *parent = nullptr);

private slots:
    // Slot name changes
    void on_mostInfluencer_clicked();
    void on_mostActive_clicked();
    void on_browseButton_clicked();

private:
    void setupUI();
    QPushButton* createStyledButton(const QString& text, const QString& style);

    // Member variables (widgets)
    QPushButton *mostinfluencer; // Widget names kept for simplicity of connecting to old logic
    QPushButton *mostactive;  // Widget names kept for simplicity of connecting to old logic
    QPushButton *browseButton;
    QTextEdit *xmlTextEdit;
};

#endif // INFLUENCE_ACTIVITY_H