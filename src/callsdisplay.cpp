#include "include/callsdisplay.h"

CallsDisplay::CallsDisplay(QObject *parent)
    : QAbstractTableModel(parent)
{
    ConnectToDB::getInstance()->moveToThread(thread);
}

QVariant CallsDisplay::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Vertical)
    {
        return section;
    }

    switch (section)
    {
    case ID:
        return ("ID");
    case DATE:
        return ("Дата");
    case TIME:
        return ("Время");
    case NUMBER:
        return ("Номер клиента");
    case COMMENT:
        return ("Комментарий");
    case WHO:
        return ("Выполнил");
    case STATUS:
        return ("Статус");
    }
    return QVariant();
}

int CallsDisplay::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_table.count();
}

int CallsDisplay::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return STATUS + 1;
}

QVariant CallsDisplay::data(const QModelIndex &index, int role) const
{
    if (m_table.isEmpty())
    {
        return QVariant();
    }
    if (!index.isValid())
    {
        return QVariant();
    }
    if ((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::BackgroundRole) &&
        (role != Qt::TextAlignmentRole))
    {
        return QVariant();
    }
    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    if (role == Qt::BackgroundRole)
    {
        if (m_table[index.row()][Column(STATUS)].toBool() == true)
        {
            QVariant completeAppeal = QColor(0, 255, 0, 100);
            return completeAppeal;
        }
        else if ((m_table[index.row()][Column(TIME)].toTime().addSecs(FIFTEEN_MINUTES) <= QTime::currentTime()) ||
                 (m_table[index.row()][Column(DATE)].toDate() < QDate::currentDate()))
        {
            QVariant warningDeadLine = QColor(255, 0, 0, 100);
            return warningDeadLine;
        }
        else if ((m_table[index.row()][Column(TIME)].toTime().addSecs(SEVEN_MINUTES) <= QTime::currentTime()) ||
                 (m_table[index.row()][Column(DATE)].toDate() < QDate::currentDate()))
        {
            QVariant dangerDeadLine = QColor(255, 255, 0, 100);
            return dangerDeadLine;
        }
    }
    if (role == Qt::DisplayRole && index.column() == STATUS)
    {
        switch (m_table[index.row()][Column(STATUS)].toBool())
        {
        case false:
            return "В работе";
        case true:
            return "Выполнено";
        }
    }
    else
    {
        return m_table[index.row()][Column(index.column())];
    }
    return QVariant();
}
bool CallsDisplay::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString row = QString::number(m_table[index.row()][Column(ID)].toInt());
    QSqlQuery tempChanged;
    if (!index.isValid() || role != Qt::EditRole || m_table.count() <= index.row())
    {
        return false;
    }

    if (role == Qt::EditRole && index.column() != STATUS)
    {
        m_table[index.row()][Column(index.column())] = value.toString();
        tempChanged.prepare("UPDATE inquiries SET comment = :comment, who = :who WHERE ticket_id = " + row);
        tempChanged.bindValue(":comment", value.toString());
        tempChanged.bindValue(":who", who);
        tempChanged.exec();
        m_table[index.row()][Column(WHO)] = who;
        QModelIndex whoIndex = createIndex(index.row(), WHO);
        emit dataChanged(index, index);
        emit dataChanged(whoIndex, whoIndex);
        return true;
    }
    if (role == Qt::EditRole && index.column() == STATUS)
    {
        m_table[index.row()][Column(index.column())] = value.toString();
        tempChanged.prepare("UPDATE inquiries SET who = :who, done_or_not = :done_or_not WHERE ticket_id = " + row);
        tempChanged.bindValue(":who", who);
        tempChanged.bindValue(":done_or_not", value.toBool());
        tempChanged.exec();
        m_table[index.row()][Column(WHO)] = who;
        QModelIndex whoIndex = createIndex(index.row(), WHO);
        QModelIndex statusIndex = createIndex(index.row(), STATUS);
        emit dataChanged(whoIndex, whoIndex);
        emit dataChanged(statusIndex, statusIndex);
        slotForReloadAppels();
        return true;
    }
    return false;
}

Qt::ItemFlags CallsDisplay::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.isValid())
    {
        if (index.column() == COMMENT || index.column() == STATUS)
        {
            flags |= Qt::ItemIsEditable;
        }
        if (index.column() == STATUS)
        {
            flags |= Qt::ItemIsUserCheckable;
        }
    }
    return flags;
}

void CallsDisplay::downloadAppels(int whichDisplay)
{
    currentWhichDisplay = whichDisplay;
    QSqlQuery showAppels;
    showAppels.exec("SELECT ticket_id, date, time, number_client, comment, who, done_or_not FROM inquiries");
    beginResetModel();
    m_table.clear();
    showAppels.last();
    do
    {
        TableData tempData;
        tempData[ID] = showAppels.value(ID).toString();
        tempData[DATE] = showAppels.value(DATE).toString();
        tempData[TIME] = showAppels.value(TIME).toString();
        tempData[NUMBER] = showAppels.value(NUMBER).toString();
        tempData[COMMENT] = showAppels.value(COMMENT).toString();
        tempData[WHO] = showAppels.value(WHO).toString();
        if (showAppels.value(STATUS).toInt() == whichDisplay)
        {

            tempData[STATUS] = showAppels.value(STATUS).toBool();

            m_table.append(tempData);
        }
    } while (showAppels.previous());
    QModelIndex startIndex = createIndex(0, 0);
    QModelIndex stopIndex = createIndex(m_table.size(), STATUS + 1);
    emit dataChanged(startIndex, stopIndex);
    endResetModel();
}

const QString CallsDisplay::checkDataBaseConnection()
{
    return ConnectToDB::dataBaseStatus(ConnectToDB::getInstance());
}

void CallsDisplay::slotForReloadAppels()
{
    downloadAppels(currentWhichDisplay);
}
