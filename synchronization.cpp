#include "synchronization.h"

Synchronization::Synchronization()
{    
    clsDatabase = new DataBase;
    clsTwitter = Twitter::getcls();

}

Synchronization::~Synchronization()
{    
    clsTwitter = 0;
    delete clsDatabase;
    delete clsTwitter;
}

void Synchronization::startSynchronization(QString userID, QString twitterID)
{
   checkUserInfo(twitterID);
   syncMyReadableUsers(userID);
}

QStringList Synchronization::getUpdatedTimelines()
{
    return updatedTimelines;
}

void Synchronization::syncMyReadableUsers(QString userID)
{
    QStringList usersToSync;
    usersToSync = clsDatabase->getUsersForSync(userID);
    for (int i=0; i< usersToSync.count(); i++)
    {
        checkUserInfo(usersToSync.at(i));
    }

    emit synchronizationFinished();
}

void Synchronization::getTimeline(QString twitterUserID)
{
    requestor = new Requests;
    parser = new Parser;
    if (clsDatabase->getLastTweetID(twitterUserID).toStdString()=="")
    {
        requestData = requestor->getRequest(GET_USER_TIMELINE,twitterUserID.toStdString(),\
                                            "&count=200");
    }
    else
    {
    requestData = requestor->getRequest(GET_USER_TIMELINE,twitterUserID.toStdString(),\
                                        "&count=200&since_id="+clsDatabase->getLastTweetID(twitterUserID).toStdString());
    }
    tweets = parser->parseTweetsToDatabase(&requestData);
    delete requestor;
    delete parser;
    tweetsToDatabase(&tweets, twitterUserID);
}

void Synchronization::tweetsToDatabase(QList<QVariant> *tweets, QString twitterUserID)
{
    QMap<QString,QVariant> tweetMap;
    QMap<QString,QVariant> userMap;
    DataBase::tweets tweetsToDB;
    QList<DataBase::tweets> dataToInsert;
    QMap<QString, QVariant>::const_iterator user;
    if (tweets->count()>0)
    {
        for ( int i = 0 ; i<tweets->count(); i++)
        {
           tweetMap = tweets->at(i).toMap();
           tweetsToDB.tweetID =tweetMap.find("id_str").value().toString();
           tweetsToDB.tweetTime = parser->dateFormat(tweetMap.find("created_at").value());
           tweetsToDB.text = tweetMap.find("text").value().toString();
           tweetsToDB.twitterUserID = twitterUserID;           
           user = tweetMap.find("user");
           userMap = user.value().toMap();
           tweetsToDB.username= userMap.find("screen_name").value().toString();
           dataToInsert.append(tweetsToDB);
        }
      clsDatabase->insertTweetsToDatabase(&dataToInsert);
      getTimeline(twitterUserID);
    }
}

void Synchronization::checkUserInfo(QString twitterUserID)
{
    requestor = new Requests;
    parser = new Parser;
    requestData = requestor->getRequest(GET_USER_BY_ID,twitterUserID.toStdString(),"");
    Twitter::userData twitterUserInfo = parser->userinfo(&requestData);
    Twitter::userData databaseUserInfo = clsDatabase->getData(twitterUserID,BY_TWITTER_ID);
    delete requestor;
    delete parser;
    bool needUpdate = false;
    if (twitterUserInfo.name != databaseUserInfo.name)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"userName",twitterUserInfo.name);
    }
    if (twitterUserInfo.screen_name != databaseUserInfo.screen_name)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"displayName",twitterUserInfo.screen_name);
    }
    if (twitterUserInfo.description != databaseUserInfo.description)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"desription",twitterUserInfo.description);
    }
    if (twitterUserInfo.profile_image_url != databaseUserInfo.profile_image_url)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"imageUrl",twitterUserInfo.profile_image_url);
        clsDatabase->updateUserData(twitterUserID,"image",twitterUserInfo.profile_image_data);
    }
    if (twitterUserInfo.friends_count != databaseUserInfo.friends_count)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"friends",twitterUserInfo.friends_count);
    }
    if (twitterUserInfo.followers_count != databaseUserInfo.followers_count)
    {
        if(!needUpdate)
            needUpdate = true;
        clsDatabase->updateUserData(twitterUserID,"followers",twitterUserInfo.followers_count);
    }
    if (twitterUserInfo.statuses_count != databaseUserInfo.statuses_count)
    {
        if(!needUpdate)
            needUpdate = true;
        updatedTimelines.append(databaseUserInfo.twitterID);
        clsDatabase->updateUserData(twitterUserID,"tweets",twitterUserInfo.statuses_count);
        getTimeline(twitterUserID);
    }
    if (needUpdate)
        updatedUsers.append(databaseUserInfo.twitterID);
}

QStringList Synchronization::getUpdatedUsers()
{
    return updatedUsers;
}

