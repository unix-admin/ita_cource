CREATE TABLE readableUsers (userID INTEGER, twitterID INTEGER);
CREATE TABLE users (userID INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, userName VARCHAR (255), twitterID INTEGER, accessTokenKey VARCHAR (255), accessTokenSecret VARCHAR (255), displayName VARCHAR (150), desription VARCHAR (255), image BLOB, imageUrl VARCHAR (255), tweets INTEGER, friends INTEGER, followers INTEGER, usertype INT);
CREATE TABLE settings (userID INTEGER, timelineTweetsByPage INTEGER, searchTweetsByPage INTEGER, searchUsersByPage INTEGER, searchTweetsToDatabase INTEGER, refreshTime INTEGER);
CREATE TABLE searches (userID INTEGER, hashtag VARCHAR (255), lastTweetID INTEGER);
CREATE TABLE tweets (tweetID INTEGER, tweetTime VARCHAR (100), username VARCHAR (255), text VARCHAR (255), userID INTEGER DEFAULT (0), searchID INTEGER DEFAULT (0));
