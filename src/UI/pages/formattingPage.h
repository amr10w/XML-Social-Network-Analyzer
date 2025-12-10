#ifndef FORMATTINGPAGE_H
#define FORMATTINGPAGE_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class FormattingPage; }
QT_END_NAMESPACE

class FormattingPage : public QWidget
{
    Q_OBJECT

public:
    explicit FormattingPage(QWidget *parent = nullptr);
    ~FormattingPage();

private slots:
    void onBrowseFile();
    void onPrettifyXML();
    void onDownload();
    void onBackToOperations();

private:
    Ui::FormattingPage *ui;
    QString currentFilePath;
    QString inputXML;
    QString outputXML;
    
    void showMessage(const QString& message, bool isError = false);
    void updateOutputVisibility();
};

#endif