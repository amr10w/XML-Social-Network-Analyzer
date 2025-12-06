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
    void formattingClicked();
    void minifyClicked();
    void prettifyClicked();
    void convertToJsonClicked();
    void compressClicked();
    void decompressClicked();
    void backToWelcomeClicked();

private slots:
    void on_btnVerification_clicked();
    void on_btnFormatting_clicked();
    void on_btnMinify_clicked();
    void on_btnPrettify_clicked();
    void on_btnConvertJson_clicked();
    void on_btnCompress_clicked();
    void on_btnDecompress_clicked();
    void on_btnBack_clicked();

private:
    Ui::ProcessingPage *ui;
};

#endif // PROCESSINGPAGE_H