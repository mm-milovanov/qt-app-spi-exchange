#ifndef CONNECTION_PAGE_GUI_H
#define CONNECTION_PAGE_GUI_H

#include <io_port_interface.h>

#include <proto/exchange.h>
#include <QObject>
#include <QTimer>

class QQmlApplicationEngine;

namespace nvg {
class SerialPortGUI;
class SerialPortArgs;
class SerialPort;
}

class ConnectionPageGUI : public QObject, public ExchangeListener
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectionChanged)

public:
    explicit ConnectionPageGUI(QQmlApplicationEngine& engine,
                               ExchangeClient* client,
                               QObject *parent = nullptr        );

public slots:
    void onSerialConnectReq(const nvg::SerialPortArgs& args);
    void onSerialDisconnectReq();
    void onConnectionChanged(nvg::IIOPort::ConnectionState state);
    void onTimerEvent();

    bool isConnected() {
        return m_isConnected;
    }

signals:
    Q_SIGNAL void connectionChanged(bool);
    Q_SIGNAL void getEchoResponse();

protected:
    void onCmdExecuted(QByteArray&) override;
    void onCmdFailed(QByteArray&) override;

private:
    void sendEchoRequest();

    nvg::SerialPort*    m_serialPort;
    nvg::SerialPortGUI* m_serialPortGUI;
    ExchangeClient* m_exchangeClient;
    bool m_isConnected;
    bool m_isDeviceResponded;
    QTimer m_timer;
};

#endif // CONNECTION_PAGE_GUI_H
