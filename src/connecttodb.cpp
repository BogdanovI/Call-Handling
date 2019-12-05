#include "include/connecttodb.h"

QString ConnectToDB::checkConnect()
{
    bool checkConnect = db.open();
    QString answerDB;
    if (!checkConnect)
    {
        answerDB = db.lastError().text();
    }
    else
    {
        answerDB = "Connected";
    }
    return answerDB;
}

ConnectToDB::ConnectToDB(QString _dbName)
    : dbName(_dbName)
{
    db.setDatabaseName(dbName);
    db.open();
}

ConnectToDB::ConnectToDB(QString _hostName, QString _dbName, QString _uName, QString _uPass, int _sPort)
    : hostName(_hostName)
    , dbName(_dbName)
    , uName(_uName)
    , uPass(_uPass)
    , sPort(_sPort)
{
    db.setHostName(hostName);
    db.setPort(sPort);
    db.setDatabaseName(dbName);
    db.setUserName(uName);
    db.setPassword(uPass);
    db.open();
}
