#ifndef VERIFICATIONPAGE_H
#define VERIFICATIONPAGE_H

#include <QWidget>

namespace Ui {
class VerificationPage;
}

class VerificationPage : public QWidget
{
    Q_OBJECT

public:
    explicit VerificationPage(QWidget *parent = nullptr);
    ~VerificationPage();

signals:
    // Signal to notify the main window to switch pages
    void backClicked();

private slots:
    // Auto-connected slots based on your UI object names
    void on_btnBack_clicked();
    void on_btnBrowse_clicked();
    void on_btnVerify_clicked();

private:
    Ui::VerificationPage *ui;
};

#endif // VERIFICATIONPAGE_H