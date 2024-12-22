#include <gui/connection_page_gui.h>
#include <gui/serial_port_gui.h>
#include <proto/exchange.h>
#include <serial_port.h>
#include <proto/proto.h>
#include <proto/crc16.h>

#include <QQmlApplicationEngine>
#include <QtQml>

ConnectionPageGUI::ConnectionPageGUI(QQmlApplicationEngine& engine,
                                     ExchangeClient* client,
                                     QObject *parent)
    : QObject{parent},
    m_exchangeClient(client),
    m_isConnected(false),
    m_isDeviceResponded(false)
{
    m_serialPortGUI = new nvg::SerialPortGUI(this);
    engine.rootContext()->setContextProperty(
        "serialPortGUI", m_serialPortGUI
    );

    QObject::connect(m_serialPortGUI, &nvg::SerialPortGUI::SerialConnectReq,
                     this, &ConnectionPageGUI::onSerialConnectReq);
    QObject::connect(m_serialPortGUI, &nvg::SerialPortGUI::SerialDisconnectReq,
                     this, &ConnectionPageGUI::onSerialDisconnectReq);
    QObject::connect(&m_timer, &QTimer::timeout,
                     this, &ConnectionPageGUI::onTimerEvent);
}

void ConnectionPageGUI::onSerialConnectReq(const nvg::SerialPortArgs& args)
{
    m_serialPort = new nvg::SerialPort(args);

    if (m_serialPort->open()) {
        m_serialPortGUI->setConnected(true);

        m_exchangeClient->setSerialPort(m_serialPort);

        m_timer.start(1000);

        QObject::connect(m_serialPort, &nvg::SerialPort::connectionChanged,
                         this, &ConnectionPageGUI::onConnectionChanged);
    } else
        delete m_serialPort;
}

void ConnectionPageGUI::onSerialDisconnectReq(){
    if (!this->m_serialPortGUI->getConnected())
        return;

    m_timer.stop();

    m_exchangeClient->unsetSerialPort();

    QObject::disconnect(m_serialPort, &nvg::SerialPort::connectionChanged,
                        this, &ConnectionPageGUI::onConnectionChanged);

    delete this->m_serialPort;
    this->m_serialPortGUI->setConnected(false);
}

void ConnectionPageGUI::onConnectionChanged(nvg::IIOPort::ConnectionState state) {
    bool isConnectedGUI = this->m_serialPortGUI->getConnected();
    bool isConnected = (state == nvg::IIOPort::ConnectionState::CONNECTED);
    if (isConnectedGUI && !isConnected)
        this->onSerialDisconnectReq();
}

void ConnectionPageGUI::sendEchoRequest() {
    QByteArray pkg;
    QDataStream stream(&pkg, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << FROM_CTRL_PACKAGE_HEADER;
    stream << (uint16_t)ExchangeCmdCode::ECHO;
    stream << (uint16_t)0x0;
    stream << (uint16_t)0x0;

    for (int i=0; i<128; i++)
        stream << (uint8_t)0x0;

    stream << (uint16_t)0x0;
    stream << (uint16_t)get_crc16_table(pkg);
    stream << FROM_CTRL_PACKAGE_FOOTER;

    m_exchangeClient->sendCmd(pkg, this);
}

void ConnectionPageGUI::onTimerEvent() {
    if (!m_isDeviceResponded && m_isConnected) {
        m_isConnected = false;
        emit connectionChanged(false);
    }

    if (m_isDeviceResponded) {
        if (!m_isConnected) {
            m_isConnected = true;
            emit connectionChanged(true);
        }
    }

    m_isDeviceResponded = false;
    this->sendEchoRequest();
}

void ConnectionPageGUI::onCmdExecuted(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdExecuted: "
             << "received \'"
             << msg << "\'";

    proto_pkg_t* pkg = (proto_pkg_t*)msg.data();
    switch ((ExchangeCmdCode)pkg->reg1) {
    case ExchangeCmdCode::ECHO: {
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "echo received";
        emit getEchoResponse();
        m_isDeviceResponded = true;
        break;
    }

    default:
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "WTF!!! Get invalid cmd type!";
        break;
    }
}

void ConnectionPageGUI::onCmdFailed(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdFailed: "
             << msg;
}
