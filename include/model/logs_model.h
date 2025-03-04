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
    typedef struct log_msg_t {
        QString time;
        QString tag;
        QString msg;
        msg_type_e type;
    } log_msg_t;

    /* Private fields */
    QList<log_msg_t> m_logMsgs;
};

#endif // LOGS_MODEL_H
