#include "include/connecttodb.h"

QString ConnectToDB::dataBaseStatus(ConnectToDB *database)
{
    QString lastMessage;
    if (!database->db.open())
    {
        lastMessage = database->db.lastError().text();
    }
    else
    {
        lastMessage = "Connected";
    }
    return lastMessage;
}

ConnectToDB *ConnectToDB::getInstance()
{
    static ConnectToDB instance;
    return &instance;
}

ConnectToDB::ConnectToDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    dbName = "./appeals.sql";
    db.setDatabaseName(dbName);
    db.open();
}
