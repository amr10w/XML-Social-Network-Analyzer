#include "graphPage.h"
#include "ui_graphPage.h"
#include "../../core/Graph.h"
#include "../../core/GraphVisualizer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QPainter>
#include <QScrollBar>
#include <QRandomGenerator>
#include <QtMath>
#include <QTemporaryDir>
#include <QDir>
#include <QImageReader>

GraphPage::GraphPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphPage)
{
    ui->setupUi(this);
    // setFixedSize(900, 750);
    // Initially hide output section
    ui->outputLabel->setVisible(false);
    ui->graphScrollArea->setVisible(false);
    ui->graphDisplayLabel->setVisible(false);
    ui->downloadButton->setVisible(false);
    
    // Set placeholder
    ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    
    // Connect signals
    connect(ui->backButton, &QPushButton::clicked, this, &GraphPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &GraphPage::onBrowseFile);
    connect(ui->generateButton, &QPushButton::clicked, this, &GraphPage::onGenerateGraph);
    connect(ui->downloadButton, &QPushButton::clicked, this, &GraphPage::onDownloadGraph);
}

GraphPage::~GraphPage()
{
    delete ui;
}

void GraphPage::onBackToOperations()
{
    // this->close();
    emit backClicked();
}

void GraphPage::onBrowseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse XML File"),
        QString(),
        tr("XML Files (*.xml);;All Files (*)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    QTextStream in(&file);
    inputXML = in.readAll();
    file.close();
    
    currentFilePath = fileName;
    ui->inputTextEdit->setPlainText(inputXML);
    
    // Hide output when new file is loaded
    ui->outputLabel->setVisible(false);
    ui->graphScrollArea->setVisible(false);
    ui->graphDisplayLabel->setVisible(false);
    ui->downloadButton->setVisible(false);
    
    // QMessageBox::information(this, "Success", "File loaded successfully!");
}

void GraphPage::onGenerateGraph()
{
    inputXML = ui->inputTextEdit->toPlainText();
    
    if (inputXML.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        // Generate the graph visualization using your existing Graph class
        generateGraphImage();
        
        // Display in UI
        if (!graphImage.isNull()) {
            ui->graphDisplayLabel->setPixmap(graphImage.scaled(
                ui->graphDisplayLabel->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
            
            // Show output section
            updateOutputVisibility();
            
            // QMessageBox::information(this, "Success", "Graph generated successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to generate graph image!");
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to generate graph: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while generating graph");
    }
}

void GraphPage::generateGraphImage()
{
    try {
        // Convert QString to std::string
        std::string xmlContent = inputXML.toStdString();
        
        // Create Graph object from XML content
        Graph graph(xmlContent);
        
        // Create a temporary directory for the output
        QTemporaryDir tempDir;
        if (!tempDir.isValid()) {
            throw std::runtime_error("Failed to create temporary directory");
        }
        
        // Generate output filename
        QString tempImagePath = tempDir.path() + "/graph_output.png";
        std::string outputFile = tempImagePath.toStdString();
        
        // Use the Visualizer function to generate the graph
        bool success = Visualizer(graph, outputFile);
        
        if (success) {
            // Load the generated image
            QImageReader reader(tempImagePath);
            reader.setAutoTransform(true);
            
            QImage image = reader.read();
            if (image.isNull()) {
                throw std::runtime_error("Failed to load generated graph image");
            }
            
            // Convert to QPixmap
            graphImage = QPixmap::fromImage(image);
            
            // Store the path for downloading
            lastGeneratedImagePath = tempImagePath;
            
        } else {
            // Fallback: Generate a simple visualization using Qt
            generateFallbackGraphImage(graph);
        }
        
    } catch (const std::exception& e) {
        // If Graph creation or visualization fails, show error
        throw std::runtime_error(std::string("Graph generation error: ") + e.what());
    }
}

void GraphPage::generateFallbackGraphImage(const Graph& graph)
{
    // Fallback visualization if Graphviz is not available
    int width = 1200;
    int height = 900;
    graphImage = QPixmap(width, height);
    graphImage.fill(QColor(30, 30, 30)); // Dark background
    
    QPainter painter(&graphImage);
    painter.setRenderHint(QPainter::Antialiasing);
    
    GRAPH graphData = graph.getGraph();
    
    if (graphData.empty()) {
        // Draw "No data" message
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(QRectF(0, 0, width, height), 
                        Qt::AlignCenter, 
                        "No users found in XML");
        painter.end();
        return;
    }
    
    // Calculate node positions in a circular layout
    struct NodeInfo {
        int id;
        QPointF position;
    };
    
    std::vector<NodeInfo> nodes;
    int nodeCount = graphData.size();
    
    // Center of the canvas
    QPointF center(width / 2.0, height / 2.0);
    double radius = std::min(width, height) * 0.35;
    
    // Position nodes in a circle
    int index = 0;
    for (const auto& entry : graphData) {
        double angle = (2.0 * M_PI * index) / nodeCount;
        QPointF pos(
            center.x() + radius * cos(angle),
            center.y() + radius * sin(angle)
        );
        nodes.push_back({entry.first, pos});
        index++;
    }
    
    // Draw edges first (so they appear behind nodes)
    painter.setPen(QPen(QColor(221, 221, 221), 2)); // Light gray edges
    painter.setBrush(QColor(221, 221, 221));
    
    for (const auto& entry : graphData) {
        int fromId = entry.first;
        const std::vector<int>& toIds = entry.second;
        
        // Find the position of the 'from' node
        auto fromIt = std::find_if(nodes.begin(), nodes.end(), 
            [fromId](const NodeInfo& n) { return n.id == fromId; });
        
        if (fromIt == nodes.end()) continue;
        
        for (int toId : toIds) {
            // Find the position of the 'to' node
            auto toIt = std::find_if(nodes.begin(), nodes.end(), 
                [toId](const NodeInfo& n) { return n.id == toId; });
            
            if (toIt == nodes.end()) continue;
            
            QPointF start = fromIt->position;
            QPointF end = toIt->position;
            
            // Calculate arrow direction
            double angle = atan2(end.y() - start.y(), end.x() - start.x());
            
            // Shorten the line to stop at node border
            double nodeRadius = 35;
            QPointF adjustedEnd(
                end.x() - nodeRadius * cos(angle),
                end.y() - nodeRadius * sin(angle)
            );
            
            QPointF adjustedStart(
                start.x() + nodeRadius * cos(angle),
                start.y() + nodeRadius * sin(angle)
            );
            
            // Draw line
            painter.drawLine(adjustedStart, adjustedEnd);
            
            // Draw arrowhead
            double arrowSize = 12;
            QPointF arrowP1 = adjustedEnd - QPointF(
                cos(angle + M_PI / 6) * arrowSize,
                sin(angle + M_PI / 6) * arrowSize
            );
            QPointF arrowP2 = adjustedEnd - QPointF(
                cos(angle - M_PI / 6) * arrowSize,
                sin(angle - M_PI / 6) * arrowSize
            );
            
            QPolygonF arrowHead;
            arrowHead << adjustedEnd << arrowP1 << arrowP2;
            painter.drawPolygon(arrowHead);
        }
    }
    
    // Draw nodes
    for (const auto& node : nodes) {
        // Draw circle
        painter.setBrush(QColor(78, 137, 255)); // Blue nodes
        painter.setPen(QPen(QColor(60, 100, 200), 3));
        painter.drawEllipse(node.position, 35, 35);
        
        // Draw user ID
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(12);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(
            QRectF(node.position.x() - 35, node.position.y() - 35, 70, 70),
            Qt::AlignCenter,
            QString::number(node.id)
        );
    }
    
    // Add title
    painter.setPen(Qt::white);
    QFont titleFont = painter.font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(QRectF(0, 20, width, 40), Qt::AlignCenter, "Social Network Graph");
    
    // Add info text
    QFont infoFont = painter.font();
    infoFont.setPointSize(12);
    infoFont.setBold(false);
    painter.setFont(infoFont);
    painter.setPen(QColor(180, 180, 180));
    painter.drawText(QRectF(0, 60, width, 30), Qt::AlignCenter, 
                    QString("Users: %1 | Connections: %2").arg(nodeCount).arg(countTotalEdges(graphData)));
    
    painter.end();
}

int GraphPage::countTotalEdges(const GRAPH& graphData) const
{
    int totalEdges = 0;
    for (const auto& entry : graphData) {
        totalEdges += entry.second.size();
    }
    return totalEdges;
}

void GraphPage::updateOutputVisibility()
{
    bool hasOutput = !graphImage.isNull();
    ui->outputLabel->setVisible(hasOutput);
    ui->graphScrollArea->setVisible(hasOutput);
    ui->graphDisplayLabel->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}

void GraphPage::onDownloadGraph()
{
    if (graphImage.isNull()) {
        QMessageBox::warning(this, "Warning", "No graph to download!");
        return;
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Graph Image"),
        "social_network_graph.png",
        tr("PNG Images (*.png);;JPEG Images (*.jpg);;All Files (*)")
    );
    
    if (saveFileName.isEmpty())
        return;
    
    if (graphImage.save(saveFileName)) {
        // QMessageBox::information(this, "Success", "Graph saved successfully to:\n" + saveFileName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to save graph image!");
    }
}

void GraphPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } else {
        // QMessageBox::information(this, "Success", message);
    }
}
