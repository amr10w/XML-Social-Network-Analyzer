#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include <QDialog>

namespace Ui {
class AboutPage;
}

class AboutPage : public QDialog
{
    Q_OBJECT

public:
    explicit AboutPage(QWidget *parent = nullptr);
    ~AboutPage();

private slots:
    // This slot handles the "Meet Our Team" button click
    void on_btnTeam_clicked();

private:
    Ui::AboutPage *ui;
};

#endif // ABOUTPAGE_H