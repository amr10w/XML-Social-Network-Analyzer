#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H

#include <QWidget>
#include <QString>
#include <QPixmap>
#include "../../core/Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GraphPage; }
QT_END_NAMESPACE

class GraphPage : public QWidget
{
    Q_OBJECT

public:
    explicit GraphPage(QWidget *parent = nullptr);
    ~GraphPage();

private slots:
    void onBrowseFile();
    void onGenerateGraph();
    void onDownloadGraph();
    void onBackToOperations();

private:
    Ui::GraphPage *ui;
    QString currentFilePath;
    QString inputXML;
    QPixmap graphImage;
    QString lastGeneratedImagePath;
    
    void generateGraphImage();
    void generateFallbackGraphImage(const Graph& graph);
    void updateOutputVisibility();
    void showMessage(const QString& message, bool isError = false);
    int countTotalEdges(const GRAPH& graphData) const;
};

#endif