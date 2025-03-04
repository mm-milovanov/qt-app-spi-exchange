#include <model/logs_model.h>

#include <QTime>

enum role_e {
    TIME = Qt::UserRole,
    TAG,
    MSG,
    TYPE
};

LogsModel::LogsModel(QObject *parent)
    : QAbstractListModel{parent}
{ }

void LogsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_logMsgs.size());
    m_logMsgs.clear();
    endRemoveRows();
}

void LogsModel::append(QString tag, QString msg, msg_type_e type) {
    if (m_logMsgs.size() > 1000) {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_logMsgs.pop_front();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    QString time = QTime::currentTime().toString("hh:mm:ss:zzz");
    m_logMsgs.append({time, tag, msg, type});
    endInsertRows();
}

QHash<int,QByteArray> LogsModel::roleNames() const {
    return {{TIME, "time"}, {TAG, "tag"}, {MSG, "msg"}, {TYPE, "type"}};
}

int LogsModel::rowCount(const QModelIndex& parent) const {
    return (parent.isValid()) ? 0 : m_logMsgs.size();
}

QVariant LogsModel::data(const QModelIndex& index, int role) const
{
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();

    auto& item = m_logMsgs.at(index.row());
    switch (role) {
    case TIME:
        return item.time;
    case TAG:
        return item.tag;
    case MSG:
        return item.msg;
    case TYPE:
        return item.type;
    }

    return QVariant();
}
