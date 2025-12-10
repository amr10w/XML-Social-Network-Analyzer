#ifndef MINIFYINGPAGE_H
#define MINIFYINGPAGE_H

#include <QWidget>
#include <QString>
#include <QScrollArea>
QT_BEGIN_NAMESPACE
namespace Ui { class MinifyingPage; }
QT_END_NAMESPACE

class MinifyingPage : public QScrollArea
{
    Q_OBJECT

public:
    explicit MinifyingPage(QWidget *parent = nullptr);
    ~MinifyingPage();

private slots:
    void onBackToOperations();
    void onBrowseFile();
    void onMinifyXML();
    void onDownload();

private:
    Ui::MinifyingPage *ui;
    QString inputXML;
    QString outputXML;
    QString currentFilePath;
    qint64 originalSize;
    qint64 minifiedSize;

    void updateStatistics();
    void updateOutputVisibility();
    void showMessage(const QString& message, bool isError = false);
};

#endif