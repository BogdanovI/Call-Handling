#ifndef CONNECTTODB_H
#define CONNECTTODB_H
#include <QString>
#include <QtSql>

class ConnectToDB
{
private:
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString hostName, dbName, uName, uPass;
    int sPort;

public:
    QString checkConnect();
    ConnectToDB(QString _dbName);
    ConnectToDB(QString _hostName, QString _dbName, QString _uName, QString _uPass, int _sPort);
};

#endif // CONNECTTODB_H
