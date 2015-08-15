#include "tweetssearch.h"
#include "ui_tweetssearch.h"
TweetsSearch::TweetsSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TweetsSearch)
{    
    ui->setupUi(this);
}

TweetsSearch::~TweetsSearch()
{
    delete clsParser;
    delete movie;
    delete ui;
}

void TweetsSearch::prepare()
{
    twitter = Twitter::getcls();
    clsParser = new Parser;
    ui->loadProgress->setVisible(false);
    ui->results->setReadOnly(true);
    connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(startSearch()));
    connect(ui->next,SIGNAL(clicked(bool)),this,SLOT(nextResults()));
    connect(ui->previous,SIGNAL(clicked(bool)),this,SLOT(previousResults()));
    movie = new QMovie(":/data/images/loading.gif");    
    ui->loadProgress->setMovie(movie);
    nextResultsParameters = "";
    next = false;
    show();
}

void TweetsSearch::startSearch()
{

        showLoadingImage(ON);
        QString tweets;        
        Requests *twitterRequest = new Requests;
        QByteArray request;
        request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100");
        tweets = clsParser->parseTweets(&request);
        maxTweet = clsParser->maxTweetID;
        minTweet = clsParser->minTweetID;        
        delete twitterRequest;
        ui->results->setText(tweets);
        scroolPosition = ui->results->verticalScrollBar()->value();
        showLoadingImage(OFF);
}

void TweetsSearch::nextResults()
{
    ui->results->clear();
    showLoadingImage(ON);
    QString tweets;
    Requests *twitterRequest = new Requests;
    QByteArray request;
    request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100&max_id="+minTweet.toStdString());
    qDebug() << minTweet;
    tweets = clsParser->parseTweets(&request);
    maxTweet = clsParser->maxTweetID;
    minTweet = clsParser->minTweetID;
    showLoadingImage(OFF);
    ui->results->setText(tweets);
    delete twitterRequest;

}

void TweetsSearch::previousResults()
{
    ui->results->clear();
    showLoadingImage(ON);
    QString tweets;
    Requests *twitterRequest = new Requests;
    QByteArray request;
    request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100&since_id="+maxTweet.toStdString());
    qDebug() << minTweet;
    tweets = clsParser->parseTweets(&request);
    maxTweet = clsParser->maxTweetID;
    minTweet = clsParser->minTweetID;
    ui->results->setText(tweets);
    delete twitterRequest;
    showLoadingImage(OFF);
}

void TweetsSearch::showLoadingImage(loadingImage signal)
{
    switch (signal) {
    case ON:
        ui->loadProgress->setVisible(true);
        movie->start();
        break;
    case OFF:
        movie->stop();
        ui->loadProgress->setVisible(false);
        break;
    }
}
