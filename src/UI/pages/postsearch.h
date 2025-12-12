#ifndef POSTSEARCH_H
#define POSTSEARCH_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

class PostSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostSearch(QWidget *parent = nullptr);

private slots:
    void on_searchByTopics_clicked();
    void on_searchByWords_clicked();
    void on_browseButton_clicked();

private:
    QPushButton *browseButton;
    QTextEdit *xmlTextEdit;
    QPushButton *postsearchByTopics;
    QPushButton *postsearchByWords;

    QPushButton* createStyledButton(const QString& text, const QString& style);
    void setupUI();
};

#endif // POSTSEARCH_H
