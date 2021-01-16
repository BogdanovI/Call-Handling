#ifndef CONNECTTODB_H
#define CONNECTTODB_H
#include <QString>
#include <QtSql>
#include <QObject>

class ConnectToDB : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery setChange;
    ConnectToDB();
public:
    static QString dataBaseStatus(ConnectToDB *database);
    static ConnectToDB *getInstance();
};

#endif // CONNECTTODB_H
