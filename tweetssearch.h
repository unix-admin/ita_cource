#ifndef TWEETSSEARCH_H
#define TWEETSSEARCH_H

#include <QWidget>
#include "twitter.h"
//#include <QUrl>
//#include <QMovie>
//#include <QMap>
//#include <QJsonDocument>
#include "parser.h"
#include "requests.h"
namespace Ui {
class TweetsSearch;
}

class TweetsSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TweetsSearch(QWidget *parent = 0);
    ~TweetsSearch();
    void prepare(Twitter *clsTwitter);

private:
    Ui::TweetsSearch *ui;
    Twitter *twitter;
    QMovie* movie;
    Parser *clsParser;
    std::string nextResultsParameters;
private slots:
    void startSearch();
    void tweetSearchFinished();
    void replyFinished();
    void nextResults();
private:
    void parseSearchedTweets(QMap<QString,QVariant> searchedTweets);
};

#endif // TWEETSSEARCH_H
