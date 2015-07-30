#include "tweetssearch.h"
#include "ui_tweetssearch.h"

TweetsSearch::TweetsSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TweetsSearch)
{
    clsParser = new Parser;
    ui->setupUi(this);
}

TweetsSearch::~TweetsSearch()
{
    delete clsParser;
    delete movie;
    delete ui;
}

void TweetsSearch::prepare(Twitter *clsTwitter)
{
    twitter = clsTwitter;
    ui->loadProgress->setVisible(false);
    ui->searchedResults->setReadOnly(true);
    connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(startSearch()));
    connect(ui->next,SIGNAL(clicked(bool)),this,SLOT(nextResults()));
    movie = new QMovie(":/data/images/loading.gif");    
    ui->loadProgress->setMovie(movie);
    nextResultsParameters = "";
    show();
}

void TweetsSearch::startSearch()
{
      ui->searchedResults->clear();
      Requests *twitterRequest = new Requests;
      ui->loadProgress->setVisible(true);
      movie->start();
      QByteArray request;
      request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),twitter);
      movie->stop();
      ui->loadProgress->setVisible(false);
      ui->searchedResults->setHtml(clsParser->parseTweets(&request));
      delete twitterRequest;

}

void TweetsSearch::tweetSearchFinished()
{   
}

void TweetsSearch::replyFinished()
{

}

void TweetsSearch::nextResults()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished()));
    ui->searchedResults->clear();
    QNetworkRequest request;
    request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/search/tweets.json",nextResultsParameters));
    QNetworkReply* reply= manager->get(request);
    ui->loadProgress->setVisible(true);
    movie->start();
    connect(reply, SIGNAL(finished()),this, SLOT(tweetSearchFinished()));
}
