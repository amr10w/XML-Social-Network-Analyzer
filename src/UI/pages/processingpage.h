#ifndef PROCESSINGPAGE_H
#define PROCESSINGPAGE_H

#include <QWidget>

namespace Ui {
class ProcessingPage;
}

class ProcessingPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessingPage(QWidget *parent = nullptr);
    ~ProcessingPage();
    
signals:
    void verificationClicked();
    // void formattingClicked(); // Removed
    void minifyClicked();
    void prettifyClicked();
    void convertToJsonClicked();
    void compressClicked();
    void decompressClicked();
    void graphClicked();
    void socialClicked();
    void postSearchClicked();
    void influenceClicked();
    void backToWelcomeClicked();

private slots:
    void on_btnVerification_clicked();
    // void on_btnFormatting_clicked(); // Removed
    void on_btnMinify_clicked();
    void on_btnPrettify_clicked();
    void on_btnConvertJson_clicked();
    void on_btnCompress_clicked();
    void on_btnDecompress_clicked();
    void on_btnGraph_clicked();
    void on_btnSocial_clicked();
    void on_btnPostSearch_clicked();
    void on_btnInfluence_clicked();
    void on_btnBack_clicked();

private:
    Ui::ProcessingPage *ui;
};

#endif // PROCESSINGPAGE_H