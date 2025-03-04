#include <gui/spi_page_gui.h>
#include <proto/proto.h>
#include <proto/crc16.h>

#include <QDebug>

SpiPageGUI::SpiPageGUI(ExchangeClient* exchange,
                       QObject *parent          ) :
    QObject{parent},
    m_exchange(exchange)
{
    m_regsConf[0x01] = {0,  0b01};
    m_regsConf[0x02] = {0,  0b01};
    m_regsConf[0x03] = {0,  0b01};
    m_regsConf[0x04] = {1,  0b01};
    m_regsConf[0x05] = {1,  0b01};
    m_regsConf[0x06] = {32, 0b01};
    m_regsConf[0x07] = {1,  0b01};
    m_regsConf[0x08] = {4,  0b10};
    m_regsConf[0x0A] = {1,  0b10};
    m_regsConf[0x0B] = {2,  0b10};
    m_regsConf[0x0C] = {1,  0b10};
    m_regsConf[0x0D] = {32, 0b10};
    m_regsConf[0x0E] = {4,  0b01};
    m_regsConf[0x10] = {2,  0b01};
    m_regsConf[0x11] = {0,  0b01};
    m_regsConf[0x12] = {0,  0b01};
}

SpiPageGUI::~SpiPageGUI() { }

void SpiPageGUI::writeRequest(uint8_t opCode, QString str) {
    QByteArray data = QByteArray::fromHex(str.toUtf8());

    QByteArray add;
    for (int i=0; i<m_regsConf[opCode].first - data.size(); i++)
        add.push_back('\x00');
    data = add + data;

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

void SpiPageGUI::readRequest(uint8_t opCode, int size) {
    qDebug() << "SpiPageGUI::readRequest: "
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

int SpiPageGUI::getDataSize(uint8_t opCode) {
    if (m_regsConf.find(opCode) == m_regsConf.end())
        return -1;

    return m_regsConf[opCode].first;
}

int SpiPageGUI::getRwPermision(uint8_t opCode) {
    if (m_regsConf.find(opCode) == m_regsConf.end())
        return -1;

    return m_regsConf[opCode].second;
}

void SpiPageGUI::onCmdExecuted(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdExecuted: "
             << "received \'"
             << msg << "\'";

    proto_pkg_t* pkg = (proto_pkg_t*)msg.data();
    switch ((ExchangeCmdCode)pkg->reg1) {
    case ExchangeCmdCode::SPI_WRITE: {
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "spi write completed";
        break;
    }

    case ExchangeCmdCode::SPI_READ: {
        QString rxData =
            QByteArray((char*)&pkg->payload.data[1], pkg->reg2-1).toHex();
        emit receivedData(rxData);
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "spi read completed";
        break;
    }

    default:
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "WTF!!! Get invalid cmd type!";
        break;
    }
}

void SpiPageGUI::onCmdFailed(QByteArray& msg) {
    qDebug() << "SpiPageGUI::onCmdFailed: "
             << "cmd failed";

    proto_pkg_t* pkg = (proto_pkg_t*)msg.data();
    switch ((ExchangeCmdCode)pkg->reg1) {
    case ExchangeCmdCode::SPI_WRITE: {
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "spi write failed";
        break;
    }

    case ExchangeCmdCode::SPI_READ: {
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "spi read failed";
        break;
    }

    default:
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "WTF!!! Get invalid cmd type!";
        break;
    }
}
