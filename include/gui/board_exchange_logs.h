#ifndef FASTBOOT_CLIENT_LOGS_H
#define FASTBOOT_CLIENT_LOGS_H

#include <QObject>
#include <QQmlEngine>
#include <QAbstractListModel>

#include "logs_model.h"

/* Classes forward declaration */
class FastBootDevice;

class FastBootClientLogs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* model READ getModel CONSTANT)
    QML_NAMED_ELEMENT(ClientLogsCtrl)

public:
    explicit FastBootClientLogs(QObject *parent = nullptr);
    ~FastBootClientLogs();

    /* Qml invokable methods */

    // Set pointer to fastboot device
    Q_INVOKABLE void setDevice(QObject*);
    // Clear model
    Q_INVOKABLE void clear(void);
    // Save log strings to file
    Q_INVOKABLE void saveToFile(QUrl);

    /* Qml properties methods */

    // Returns pointer to LogsModel class object
    QAbstractListModel* getModel();

public slots:
    void d(QString tag, QString msg); // Insert debug type msg
    void i(QString tag, QString msg); // Insert info type msg
    void k(QString tag, QString msg); // Insert ok type msg
    void w(QString tag, QString msg); // Insert warrning type msg
    void e(QString tag, QString msg); // Insert fatal type msg

private:
    /* Private fields */
    FastBootDevice* m_device;
    LogsModel m_model;
};

#endif // FASTBOOT_CLIENT_LOGS_H
