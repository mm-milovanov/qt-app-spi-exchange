#include <proto/exchange.h>
#include <proto/parser.h>
#include <proto/crc16.h>
#include <proto/proto.h>
#include <serial_port.h>

#include <QDebug>

#define EXCHANGE_MAX_EXEC_CMD_ATTEMPTS 3

ExchangeClient::ExchangeClient(QObject *parent) :
    QObject{parent},
    m_serialPort(nullptr),
    m_status(IDLE),
    m_count(0),
    m_parser(new Parser(this)),
    m_attempt(0)
{
    m_parser->setHeader(FROM_BOARD_PACKAGE_HEADER);
    m_parser->setFooter(FROM_BOARD_PACKAGE_FOOTER);
    m_parser->setLengthOffset(-1);

    QObject::connect(m_parser, &Parser::validMsg,
                     this, &ExchangeClient::onValidMsg);
    QObject::connect(&m_timer, &QTimer::timeout,
                     this, &ExchangeClient::onTimerEvent);
}

ExchangeClient::~ExchangeClient() {
    QObject::disconnect(m_parser, &Parser::validMsg,
                     this, &ExchangeClient::onValidMsg);
    QObject::disconnect(&m_timer, &QTimer::timeout,
                     this, &ExchangeClient::onTimerEvent);

    delete m_parser;
}

void ExchangeClient::sendCmd(QByteArray& data, ExchangeListener* listener) {
    m_cmdQueue.push({data, listener});

    this->beginCmd();
}

void ExchangeClient::reset() {
    while (!m_cmdQueue.empty()) {
        auto [pkg, listener] = m_cmdQueue.front();
        m_cmdQueue.pop();

        listener->onCmdFailed(pkg);
    }

    m_count = 0;
    m_attempt = 0;
    m_status = IDLE;
    m_parser->reset();
    m_timer.stop();
}

void ExchangeClient::beginCmd() {
    if (m_cmdQueue.empty()) {
        m_status = IDLE;
        return;
    }

    if (m_serialPort == nullptr) {
        m_status = IDLE;
        return;
    }

    if (m_status != IDLE)
        return;

    // Change status
    m_status = PROCESSING;
    m_attempt = 0;

    // Get cmd from cmd queue
    QByteArray pkg = m_cmdQueue.front().first;

    // Send package
    if (m_serialPort->sendData(pkg.data(), pkg.size()))
        qDebug() << "ExchangeClient::sendCmd: "
                 << '[' << pkg.size() << ']'
                 << pkg.toHex();

    m_timer.start(1000);
}

void ExchangeClient::onDataReceived(QByteArray& data) {
    qDebug() << "ExchangeClient::onDataReceived: "
             << '[' << data.size() << ']'
              << data.toHex();

    m_parser->pushBuf(data);
    m_parser->step();
}

void ExchangeClient::onValidMsg(QByteArray& data) {
    m_timer.stop();

    qDebug() << "ExchangeClient::onValidMsg: "
             << '[' << data.size() << ']'
             << data.toHex();

    if (m_cmdQueue.empty())
        return;

    auto [cmd, listener] = m_cmdQueue.front();
    m_cmdQueue.pop();

    proto_pkg_t* pkg = (proto_pkg_t*)data.data();
    if (get_crc16_table(data.mid(0, data.size()-4)) == pkg->crc)
        listener->onCmdExecuted(data);
    else
        listener->onCmdFailed(data);

    m_status = IDLE;
    if (!m_cmdQueue.empty())
        this->beginCmd();
}

void ExchangeClient::setSerialPort(nvg::SerialPort* port) {
    m_serialPort = port;

    QObject::connect(m_serialPort, &nvg::SerialPort::dataReceived,
                     this, &ExchangeClient::onDataReceived);

    this->beginCmd();
}

void ExchangeClient::unsetSerialPort() {
    QObject::disconnect(m_serialPort, &nvg::SerialPort::dataReceived,
                     this, &ExchangeClient::onDataReceived);

    m_serialPort = nullptr;
    m_count = 0;
}

void ExchangeClient::onTimerEvent() {
    QString str;

    if (m_attempt++ < EXCHANGE_MAX_EXEC_CMD_ATTEMPTS) {
        str = "ExchangeClient::onTimerEvent: "
              "Device didn't respond! "
              "Attempt %1 of %2.";
        str = str.arg(m_attempt);
        str = str.arg(EXCHANGE_MAX_EXEC_CMD_ATTEMPTS);
        this->beginCmd();
    }
    else {
        str = "Device does not response!";
        this->reset();
    }
    qDebug() << str;
}
