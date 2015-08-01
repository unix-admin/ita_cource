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
    connect(ui->previous,SIGNAL(clicked(bool)),this,SLOT(previousResults()));
    movie = new QMovie(":/data/images/loading.gif");    
    ui->loadProgress->setMovie(movie);
    nextResultsParameters = "";
    show();
}

void TweetsSearch::startSearch()
{
      metadata = new QStringList;
      ui->searchedResults->clear();
      showLoadingImage(ON);
      Requests *twitterRequest = new Requests;      
      QByteArray request;
      QStringList results;
      request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100",twitter);
      results = clsParser->parseSearchRequest(&request);
      metadata->append(results.at(0));
      metadata->append(results.at(1));
      metadata->append(results.at(2));
      metadata->append(results.at(3));
      metadata->append(results.at(4));
      showLoadingImage(OFF);
      ui->searchedResults->setHtml(results.at(2));
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
    ui->searchedResults->clear();
    showLoadingImage(ON);
    Requests *twitterRequest = new Requests;
    QByteArray request;
    QStringList results;
    request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100&max_id="+metadata->at(3).toStdString(),twitter);
    qDebug() << metadata->at(0);
    delete metadata;
    metadata = new QStringList;
    results = clsParser->parseSearchRequest(&request);
    metadata->append(results.at(0));
    metadata->append(results.at(1));
    metadata->append(results.at(2));
    metadata->append(results.at(3));
    metadata->append(results.at(4));
    showLoadingImage(OFF);
    ui->searchedResults->setHtml(results.at(2));
    delete twitterRequest;

}

void TweetsSearch::previousResults()
{
    ui->searchedResults->clear();
    showLoadingImage(ON);
    Requests *twitterRequest = new Requests;
    QByteArray request;
    QStringList results;
    request = twitterRequest->getRequest(TWEETS_SEARCH,ui->searchField->text().toStdString(),"&count=100&since_id="+metadata->at(3).toStdString(),twitter);
    qDebug() << metadata->at(0);
    delete metadata;
    metadata = new QStringList;
    results = clsParser->parseSearchRequest(&request);
    metadata->append(results.at(0));
    metadata->append(results.at(1));
    metadata->append(results.at(2));
    metadata->append(results.at(3));
    metadata->append(results.at(4));
    showLoadingImage(OFF);
    ui->searchedResults->setHtml(results.at(2));
    delete twitterRequest;
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
