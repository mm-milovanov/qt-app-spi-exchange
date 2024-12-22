#ifndef APP_ENGINE_H
#define APP_ENGINE_H

#include <QObject>

class QQmlApplicationEngine;
class ConnectionPageGUI;
class SpiPageGUI;
class ExchangeClient;

namespace nvg {
class SerialPortGUI;
class SerialPortArgs;
class SerialPort;
}

class AppEngine : public QObject
{
    Q_OBJECT
public:
    explicit AppEngine(QQmlApplicationEngine& engine,
                       QObject *parent = nullptr);
    ~AppEngine();

signals:

private:
    ConnectionPageGUI*  m_connectionPageGUI;
    SpiPageGUI*         m_spiPageGUI;
    ExchangeClient*     m_exchangeClient;
};

#endif // APP_ENGINE_H
