#ifndef COMPRESSPAGE_H
#define COMPRESSPAGE_H

#include <QWidget>
#include <QString>

// Forward declaration of the UI namespace
QT_BEGIN_NAMESPACE
namespace Ui { class CompressPage; } 
QT_END_NAMESPACE

class CompressPage : public QWidget
{
    Q_OBJECT

public:
    explicit CompressPage(QWidget *parent = nullptr);
    ~CompressPage();

private slots:
    void onBackToOperations();
    void onBrowseFile();
    
    // This function will now call the new backend utility: compressXMLString
    void onCompress(); 
    void onDownload();

signals:
    void backToHomeClicked();

private:
    Ui::CompressPage *ui;
    QString inputQString;
    QString currentFilePath;
    qint64 originalSize;
    qint64 compressedSize;

    QByteArray outputComp;

    void updateStatistics();
    void updateOutputVisibility();
    void showMessage(const QString& message, bool isError = false);
};

#endif // COMPRESSPAGE_H