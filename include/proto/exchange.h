#ifndef EXCHANGE_CLIENT_H
#define EXCHANGE_CLIENT_H

#include <io_port_interface.h>

#include <queue>

#include <QObject>
#include <QTimer>

class QQmlApplicationEngine;
class ExchangeListener;
class Parser;

namespace nvg {
class SerialPort;
}

/* Exchange client class */
class ExchangeClient : public QObject
{
    Q_OBJECT

    enum ExchangeStatus {
        IDLE,
        PROCESSING
    };

public:
    explicit ExchangeClient(QObject *parent = nullptr);
    ~ExchangeClient();

    void sendCmd(QByteArray& data, ExchangeListener* listener);
    void reset();
    void setSerialPort(nvg::SerialPort*);
    void unsetSerialPort();

private slots:
    void onTimerEvent();
    void onDataReceived(QByteArray& data);
    void onValidMsg(QByteArray& data);

private:
    void beginCmd();

    int m_attempt;
    uint m_count;
    QTimer m_timer;
    nvg::SerialPort* m_serialPort;
    std::queue <std::pair<QByteArray, ExchangeListener*>>
        m_cmdQueue;
    ExchangeStatus m_status;
    Parser* m_parser;
};

/* Exchange listener interface */
class ExchangeListener {
protected:
    virtual void onCmdExecuted(QByteArray&) = 0;
    virtual void onCmdFailed(QByteArray&) = 0;

    friend class ExchangeClient;
};

#endif // EXCHANGE_CLIENT_H
