#include <gui/arinc_bus_check_gui.h>
#include <proto/proto.h>
#include <proto/crc16.h>

#include <QDebug>

ArincBusCheckPageGUI::ArincBusCheckPageGUI(ExchangeClient* exchange,
                                               QObject *parent) :
    QObject{parent},
    m_exchange(exchange)
{}

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
    switch ((ExchangeCmdCode)pkg->reg1) {
    case ExchangeCmdCode::SPI_WRITE: {
        qDebug() << "SpiPageGUI::onCmdExecuted: "
                 << "spi write completed";
        break;
    }

    case ExchangeCmdCode::SPI_READ: {
        QString rxData =
            QByteArray((char*)&pkg->payload.data[1], pkg->reg2-1).toHex();
        // emit receivedData(rxData);
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

void ArincBusCheckPageGUI::onCmdFailed(QByteArray& msg) {
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
