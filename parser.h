#ifndef PARSER_H
#define PARSER_H
#include <QMovie>
#include <QMap>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QVariant>


class Parser
{
public:
    Parser();
    QString parseTweets(QByteArray *data);
    std::string parseSearchMetadata(QByteArray *data);
};

#endif // PARSER_H
