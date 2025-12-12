#include "social_relations_page.h"
#include "ui_social_relations_page.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "..//..//core/Graph.h"
#include "..//..//core/Suggest.h"
#include "..//..//core/Mutual.h"

SocialRelationsPage::SocialRelationsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SocialRelationsPage)
{
    ui->setupUi(this);
    setMinimumSize(500, 650);

    // 1. Connect Navigation
    connect(ui->backButton, &QPushButton::clicked, this, &SocialRelationsPage::onBackClicked);

    // 2. Connect Tab Buttons
    connect(ui->btnTabMutual, &QPushButton::clicked, this, &SocialRelationsPage::onTabMutualClicked);
    connect(ui->btnTabSuggest, &QPushButton::clicked, this, &SocialRelationsPage::onTabSuggestClicked);

    // 3. Connect Action Buttons
    connect(ui->browseButton, &QPushButton::clicked, this, &SocialRelationsPage::onBrowseClicked);
    connect(ui->btnFindMutual, &QPushButton::clicked, this, &SocialRelationsPage::onFindMutualClicked);
    connect(ui->btnGetSuggestions, &QPushButton::clicked, this, &SocialRelationsPage::onGetSuggestionsClicked);

    // Start on Mutual Tab
    setTabActive(true);
}

SocialRelationsPage::~SocialRelationsPage()
{
    delete ui;
}

void SocialRelationsPage::onBackClicked()
{
    emit backClicked();
}

// --- Logic ---
void SocialRelationsPage::onTabMutualClicked() { setTabActive(true); }
void SocialRelationsPage::onTabSuggestClicked() { setTabActive(false); }

void SocialRelationsPage::setTabActive(bool isMutual)
{
    // Update Stacked Widget
    if (isMutual) {
        ui->stackedWidget->setCurrentWidget(ui->pageMutual);
    } else {
        ui->stackedWidget->setCurrentWidget(ui->pageSuggest);
    }

    // Update Buttons Visual State (Checked)
    ui->btnTabMutual->setChecked(isMutual);
    ui->btnTabSuggest->setChecked(!isMutual);
    
    // Clear old results
    ui->txtResult->clear();
}

// --- Browse Logic ---
void SocialRelationsPage::onBrowseClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open XML", "", "XML Files (*.xml);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            ui->txtXmlInput->setPlainText(in.readAll());
            file.close();
        }
    }
}

// --- Mutual Logic ---
void SocialRelationsPage::onFindMutualClicked()
{
    QString xml = ui->txtXmlInput->toPlainText();
    QString ids = ui->txtMutualUserIds->text();

    if (xml.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please input XML content.");
        return;
    }
    if (ids.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter User IDs.");
        return;
    }

    performMutualSearch(xml, ids);
}

void SocialRelationsPage::performMutualSearch(const QString& xml, const QString& ids)
{
  std::string tmp = ids.toStdString();
  std::vector<int> idsVector;
  for(int i = 0; i < tmp.size(); i++)
  {
    if(tmp[i] != ',') idsVector.push_back(tmp[i] - '0');
  }
  Graph graph(xml.toStdString());
  std::vector<int> mutual = computeMutualFollowers(graph, idsVector);
  std::string res = "";
  if (mutual.empty())
      res += "No mutual followers found.\n";
  else
  {
    res += "Mutual followers: ";
    for (int x : mutual)
      res += std::to_string(x) + " ";
    res += "\n";
  }
  ui->txtResult->setPlainText(QString::fromStdString(res));
}

// --- Suggest logic ---
void SocialRelationsPage::onGetSuggestionsClicked()
{
    QString xml = ui->txtXmlInput->toPlainText();
    QString id = ui->txtSuggestId->text();

    if (xml.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please input XML content.");
        return;
    }
    if (id.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a User ID.");
        return;
    }

    performSuggestion(xml, id);
}

void SocialRelationsPage::performSuggestion(const QString& xml, const QString& userId)
{
    Graph graph(xml.toStdString());
    std::vector<int> suggests = suggest(graph, userId.toInt());
    std::string res = "";
    if (suggests.empty())
        res += "No suggestions found.\n";
    else
    {
      res += "Suggested: ";
      for (int x : suggests)
          res += std::to_string(x) + " ";
      res += "\n";
    }
    ui->txtResult->setPlainText(QString::fromStdString(res));
}
