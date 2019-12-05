#ifndef CALLSDISPLAY_H
#define CALLSDISPLAY_H
#define IN_WORK 0

#include "include/connecttodb.h"
#include <QAbstractTableModel>
#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTime>

class CallsDisplay : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CallsDisplay(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    ConnectToDB *db;
    QString who;
    void downloadAppels(int whichDisplay);

public slots:
    void slotForReloadAppels();

private:
    enum Column
    {
        ID = 0,
        DATE,
        TIME,
        NUMBER,
        COMMENT,
        WHO,
        STATUS
    };
    enum timeb
    {
        SEVEN_MINUTES = 9000,
        FIFTEEN_MINUTES = 18000
    };

    typedef QHash<Column, QVariant> TableData;
    typedef QList<TableData> Table;
    Table m_table;
    int currentWhichDisplay = IN_WORK;
    QItemDelegate *delegate = new QItemDelegate;
};

#endif // CALLSDISPLAY_H
