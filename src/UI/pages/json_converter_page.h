#ifndef JSONCONVERTERPAGE_H
#define JSONCONVERTERPAGE_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class JsonConverterPage; }
QT_END_NAMESPACE

class JsonConverterPage : public QWidget
{
    Q_OBJECT

public:
    explicit JsonConverterPage(QWidget *parent = nullptr);
    ~JsonConverterPage();

private slots:
    void onBrowseFile();
    void onConvertToJSON();
    void onDownload();
    void onBackToOperations();

signals:
    void backToHomeClicked();

private:
    Ui::JsonConverterPage *ui;
    QString currentFilePath;
    QString inputXML;
    QString outputJSON;
    
    void updateOutputVisibility();
};

#endif