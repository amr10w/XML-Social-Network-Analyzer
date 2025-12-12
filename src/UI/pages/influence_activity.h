// influence_activity.h

#ifndef INFLUENCE_ACTIVITY_H
#define INFLUENCE_ACTIVITY_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

// Forward declarations for external core functions/structs (assuming they are needed)
// ...

// Class name change: PostSearch -> InfluenceAndActivity
namespace Ui {
class InfluenceAndActivity;
}

class InfluenceAndActivity : public QWidget
{
    Q_OBJECT

public:
    explicit InfluenceAndActivity(QWidget *parent = nullptr);
    ~InfluenceAndActivity();

signals:
    void backToHomeClicked();

private slots:
    void on_mostInfluencerButton_clicked();
    void on_mostActiveButton_clicked();
    void on_browseButton_clicked();
    void on_backButton_clicked();
    void on_xmlTextEdit_textChanged();

private:
    Ui::InfluenceAndActivity *ui;
    QString currentFilePath;
};

#endif // INFLUENCE_ACTIVITY_H