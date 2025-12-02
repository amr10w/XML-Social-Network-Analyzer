#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

namespace Ui {
class WelcomePage;
}

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

signals:
    // Signals to tell MainWindow what the user wants to do
    void startProcessingClicked();
    void aboutClicked();

private slots:
    void on_btnStart_clicked();
    void on_btnAbout_clicked();

private:
    Ui::WelcomePage *ui;
};

#endif // WELCOMEPAGE_H