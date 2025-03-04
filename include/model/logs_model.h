#ifndef LOGS_MODEL_H
#define LOGS_MODEL_H

#include <QAbstractListModel>
#include <QObject>

class LogsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // Log msg type
    enum msg_type_e {
        DEBUG = 0,
        INFO,
        OK,
        WARNING,
        ERROR
    };

    explicit LogsModel(QObject *parent = nullptr);

    /* Public methods */

    // Clear model
    void clear(void);
    // Add new element to the end of list
    void append(QString tag, QString msg, msg_type_e type);

    /* QAbstractListModel methods */

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

private:
    /* Private fields */
    QList<std::tuple<QString,QString,QString,msg_type_e>>
    m_logMsgs; // model list of tuple {time, tag, msg, type}
};

#endif // LOGS_MODEL_H
