#ifndef BOARD_EXCHANGE_LOGS_H
#define BOARD_EXCHANGE_LOGS_H

#include <QObject>
#include <QQmlEngine>
#include <QAbstractListModel>

#include <model/logs_model.h>

class BoardExchangeLogs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* model READ getModel CONSTANT)
    // QML_NAMED_ELEMENT(ClientLogsCtrl)

public:
    explicit BoardExchangeLogs(QObject *parent = nullptr);
    ~BoardExchangeLogs();

    /* Qml invokable methods */

    // Set pointer to fastboot device
    Q_INVOKABLE void clear(void);

    /* Qml properties methods */

    // Returns pointer to LogsModel class object
    QAbstractListModel* getModel();

public slots:
    /* Public slots */

    void d(QString tag, QString msg); // Insert debug type msg
    void i(QString tag, QString msg); // Insert info type msg
    void k(QString tag, QString msg); // Insert ok type msg
    void w(QString tag, QString msg); // Insert warrning type msg
    void e(QString tag, QString msg); // Insert fatal type msg

private:
    /* Private fields */
    LogsModel m_model;
};

#endif // BOARD_EXCHANGE_LOGS_H
