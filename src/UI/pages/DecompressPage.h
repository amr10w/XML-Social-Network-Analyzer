#ifndef DECOMPRESSPAGE_H
#define DECOMPRESSPAGE_H

#include <QWidget>
#include <QString>

// Forward declaration of the UI namespace
QT_BEGIN_NAMESPACE
namespace Ui { class DecompressPage; } 
QT_END_NAMESPACE

class DecompressPage : public QWidget
{
    Q_OBJECT

public:
    explicit DecompressPage(QWidget *parent = nullptr);
    ~DecompressPage();

signals:
    void backToProcessingClicked();

private slots:
    void onBackToOperations();
    void onBrowseFile();
    void onDecompress(); 
    void onDownload();

private:
    Ui::DecompressPage *ui;
    
    // Holds the raw byte content of the loaded compressed file (.comp)
    QByteArray compressedDataBytes; 
    
    // Holds the final decompressed XML output
    QString outputQString;
    
    // File path for smart saving/file info
    QString currentFilePath;

    std::string fileExtention;

    qint64 originalSize;    // Size of the loaded .comp file
    qint64 decompressedSize; // Size of the resulting XML
    
    // UI Helpers
    void updateStatistics();
    void updateOutputVisibility(bool visible);
    void showMessage(const QString& message, bool isError = false);
};

#endif // DECOMPRESSPAGE_H