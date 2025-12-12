#ifndef SOCIALRELATIONSPAGE_H
#define SOCIALRELATIONSPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SocialRelationsPage; }
QT_END_NAMESPACE

class SocialRelationsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SocialRelationsPage(QWidget *parent = nullptr);
    ~SocialRelationsPage();

signals:
    void backClicked();

private slots:
    // Navigation
    void onBackClicked();
    
    // Tabs
    void onTabMutualClicked();
    void onTabSuggestClicked();

    // Actions
    void onBrowseClicked();
    void onFindMutualClicked();
    void onGetSuggestionsClicked();

private:
    Ui::SocialRelationsPage *ui;

    void setTabActive(bool isMutual);
    
    // logic
    void performMutualSearch(const QString& xml, const QString& ids);
    void performSuggestion(const QString& xml, const QString& userId);
};

#endif