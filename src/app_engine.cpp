#include <app_engine.h>
#include <gui/serial_port_gui.h>
#include <gui/connection_page_gui.h>
#include <gui/spi_page_gui.h>
#include <gui/arinc_bus_check_gui.h>
#include <proto/exchange.h>
#include <src/version.h>

#include <QQmlApplicationEngine>
#include <QtQml>

AppEngine::AppEngine(QQmlApplicationEngine& engine,
                     QObject *parent                ) :
    QObject{parent}
{
    m_exchangeClient = new ExchangeClient(this);

    m_connectionPageGUI = new ConnectionPageGUI(engine, m_exchangeClient, this);
    engine.rootContext()->setContextProperty(
        "connectionPageGUI", m_connectionPageGUI
    );

    m_spiPageGUI = new SpiPageGUI(m_exchangeClient, this);
    engine.rootContext()->setContextProperty(
        "spiPageGUI", m_spiPageGUI
    );

    m_arincBusCheckPageGUI = new ArincBusCheckPageGUI(m_exchangeClient, this);
    engine.rootContext()->setContextProperty(
        "connectionCheckPageGUI", m_arincBusCheckPageGUI
        );

    engine.rootContext()->setContextProperty(
        "NVG_VERSION_STRING", NVG_VERSION_STRING
    );
    engine.rootContext()->setContextProperty(
        "NVG_VERSION_BUILD_DATE", NVG_VERSION_BUILD_DATE
    );
    engine.rootContext()->setContextProperty(
        "NVG_VERSION_BUILD_TIME", NVG_VERSION_BUILD_TIME
    );
}

AppEngine::~AppEngine() {
    delete m_exchangeClient;
    delete m_connectionPageGUI;
    delete m_spiPageGUI;
}
