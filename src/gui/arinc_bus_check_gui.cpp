#include <gui/arinc_bus_check_gui.h>

#include <proto/proto.h>
#include <proto/crc16.h>

#include <extra/idle_wait.h>

#include <QDebug>

ArincBusCheckPageGUI::ArincBusCheckPageGUI(ExchangeClient* exchange,
                                           QObject *parent) :
    QObject{parent},
    m_status{status_e::IDLE},
    m_n(0),
    m_SR(0),
    m_exchange(exchange)
{ }

QObject* ArincBusCheckPageGUI::getLogsCtrl() {
    return &m_logs;
}

void ArincBusCheckPageGUI::start() {
    if (m_status == PAUSE) {
        this->changeStatus(ACTIVE);
        m_logs.i("INFO", "Resume ARINC bus check algorithm");
        this->algorithmSendStep();
        return;
    }

    // Clear logs
    m_logs.clear();

    this->changeStatus(ACTIVE);
    m_logs.i("INFO", "Start ARINC bus check algorithm");

    m_index = m_cnt = 0;
    this->changePkgNum(0);
    m_algStatus = alg_status_e::MASTER_RESET;

    m_pkg.clear();
    QDataStream stream(&m_pkg, QDataStream::WriteOnly);
    for (int i=0; i<32; i++)
        stream << ((uint32_t)m_cnt++);
    emit this->dataChanged(m_pkg.toHex());

    this->algorithmSendStep();
}

void ArincBusCheckPageGUI::stop() {
    m_logs.i("INFO", "Stop ARINC bus check algorithm");

    if (m_status != ERROR)
        this->changeStatus(IDLE);
}


void ArincBusCheckPageGUI::pause() {
    m_logs.i("INFO", "Pause ARINC bus check algorithm");
    this->changeStatus(PAUSE);
}

int ArincBusCheckPageGUI::getStatus() {
    return ((int)m_status);
}

uint ArincBusCheckPageGUI::getPkgNum() {
    return m_n;
}

int ArincBusCheckPageGUI::getSR() {
    return m_SR;
}

QString ArincBusCheckPageGUI::getData() {
    return m_pkg.toHex();
}

void ArincBusCheckPageGUI::changeStatus(status_e status) {
    m_status = status;
    emit statusChanged(((int)status));
}

void ArincBusCheckPageGUI::changePkgNum(uint num) {
    m_n = num;
    emit this->pkgNumChanged(m_n);
}

void ArincBusCheckPageGUI::changeSR(int SR) {
    m_SR = SR;
    emit this->SRchanged(m_SR);
}

void ArincBusCheckPageGUI::algorithmSendStep() {
    switch(m_algStatus) {
    case MASTER_RESET:
        this->writeRequest(0x01, {});
        break;

    case CHECK_STATUS_REG_0x09:
        this->readRequest(0x0A, 1);
        break;

    case WRITE_PKG:
        if (m_index < 256) {
            m_logs.d("DEBUG", "Write 64 byte");
            this->writeRequest(0x0E, m_pkg.mid(m_index, 64));
            m_index += 64;
        }
        break;

    case CHECK_STATUS_REG_0x0E:
        this->readRequest(0x0A, 1);
        break;

    case READ_PKG:
        this->readRequest(0x08, 4);
        break;
    }
}

void ArincBusCheckPageGUI::algorithmRecvStep(const QByteArray& data) {
    switch(m_algStatus) {
    case MASTER_RESET:
        m_algStatus = CHECK_STATUS_REG_0x09;
        break;

    case CHECK_STATUS_REG_0x09: {
        this->changeSR((unsigned char)data[0]);
        m_logs.d("DEBUG", "Receive SR=0x" + data.toHex());
        if (data[0] != '\xff') {
            m_logs.e("ERROR",
                (m_n == 0)
                    ? "Board didn't do master reset!"
                    : "Status register does not equal 0x09!"
            );
            this->changeStatus(ERROR);
            this->stop();
            return;
        }

        m_algStatus = WRITE_PKG;
        break;
    }

    case WRITE_PKG:
        if (m_index >= 256) {

            // wait 10240 microseconds ~ 11 milliseconds
            // auto idleWait = IdleWait(11, this);
            // idleWait.start();

            m_algStatus = CHECK_STATUS_REG_0x0E;
        }

        break;

    case CHECK_STATUS_REG_0x0E:
        this->changeSR((unsigned char)data[0]);
        m_logs.d("DEBUG", "Receive SR=0x" + data.toHex());
        if (data[0] != '\xff') {
            m_logs.e("ERROR", "Data transfer timeout!");
            this->changeStatus(ERROR);
            this->stop();
            return;
        }

        m_algStatus = READ_PKG;
        break;

    case READ_PKG:
        for (int i=0; i<4; i++, m_index++)
            if (m_index < m_pkg.size() && data[i] != m_pkg[m_index]) {
                m_logs.e("ERROR", "Received data does not match the data was sended!");
                this->changeStatus(ERROR);
                this->stop();
                return;
            }

        m_logs.k("OK", "Package is valid");

        this->changePkgNum(m_n + 1);
        m_index = 0;
        m_algStatus = alg_status_e::CHECK_STATUS_REG_0x09;

        m_pkg.clear();
        QDataStream stream(&m_pkg, QDataStream::WriteOnly);
        for (int i=0; i<32; i++)
            stream << ((uint32_t)m_cnt++);
        emit this->dataChanged(m_pkg.toHex());

        // If process pause or get catch error then stop algorithm
        if (m_status != ACTIVE)
            return;

        break;
    }

    this->algorithmSendStep();
}

void ArincBusCheckPageGUI::writeRequest(uint8_t opCode, QByteArray data) {
    QByteArray pkg;
    QDataStream stream(&pkg, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << FROM_CTRL_PACKAGE_HEADER;
    stream << (uint16_t)ExchangeCmdCode::SPI_WRITE;
    stream << (uint16_t)(data.size()+1);
    stream << (uint16_t)0x0;

    stream << (uint8_t)opCode;
    stream.writeRawData(data.toStdString().data(), data.size());
    for (int i=0; i<127-data.size(); i++)
        stream << (uint8_t)0x0;

    stream << (uint16_t)0x0;
    stream << (uint16_t)get_crc16_table(pkg);
    stream << FROM_CTRL_PACKAGE_FOOTER;

    m_exchange->sendCmd(pkg, this);
}

void ArincBusCheckPageGUI::readRequest(uint8_t opCode, int size) {
    qDebug() << "ConnectionCheckPageGUI::readRequest: "
             << opCode << " " << size;

    QByteArray data;
    QDataStream stream(&data, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << FROM_CTRL_PACKAGE_HEADER;
    stream << (uint16_t)ExchangeCmdCode::SPI_READ;
    stream << (uint16_t)(size+1);
    stream << (uint16_t)0x0;

    stream << (uint8_t)opCode;
    for (int i=0; i<127; i++)
        stream << (uint8_t)0x0;

    stream << (uint16_t)0x0;
    stream << (uint16_t)get_crc16_table(data);
    stream << FROM_CTRL_PACKAGE_FOOTER;

    m_exchange->sendCmd(data, this);
}

void ArincBusCheckPageGUI::onCmdExecuted(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdExecuted: "
             << "received \'"
             << msg << "\'";

    proto_pkg_t* pkg = (proto_pkg_t*)msg.data();

    switch ((ExchangeCmdCode)pkg->reg1)
    {
    case ExchangeCmdCode::SPI_WRITE: {
        this->algorithmRecvStep();
        qDebug() << "SpiPageGUI::onCmdExecuted: spi write completed";
        break;
    }

    case ExchangeCmdCode::SPI_READ: {
        QByteArray rxData = QByteArray((char*)&pkg->payload.data[1], pkg->reg2-1);
        this->algorithmRecvStep(rxData);
        qDebug() << "SpiPageGUI::onCmdExecuted: spi read completed";
        break;
    }

    default:
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "WTF!!! Get invalid cmd type!";
        break;
    }
}

void ArincBusCheckPageGUI::onCmdFailed(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdFailed: "
             << "cmd failed";

    proto_pkg_t* pkg = (proto_pkg_t*)msg.data();

    switch ((ExchangeCmdCode)pkg->reg1)
    {
    case ExchangeCmdCode::SPI_WRITE:
        qDebug() << "SpiPageGUI::onCmdExecuted: spi write failed";
        break;

    case ExchangeCmdCode::SPI_READ:
        qDebug() << "SpiPageGUI::onCmdExecuted: spi read failed";
        break;

    default:
        qDebug() << "SpiPageGUI::onCmdExecuted: WTF!!! Get invalid cmd type!";
        break;
    }
}
