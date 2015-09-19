#ifndef TWEETSSEARCH_H
#define TWEETSSEARCH_H

#include <QWidget>
#include <QScrollBar>
#include "twitter.h"
#include "parser.h"
#include "requests.h"

enum loadingImage{
     ON=0
    ,OFF
};

namespace Ui {
class TweetsSearch;
}

class TweetsSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TweetsSearch(QWidget *parent = 0);
    ~TweetsSearch();
    void prepare();

private:
    Ui::TweetsSearch *ui;
    Twitter *twitter;
    QMovie *movie;
    Parser *clsParser;
    std::string nextResultsParameters;    
    QString maxTweet;
    QString minTweet;
    int scroolPosition;
    bool next;
private slots:
    void startSearch();
    void nextResults();
    void previousResults();   
private:    
    void showLoadingImage(loadingImage signal);
};

#endif // TWEETSSEARCH_H
