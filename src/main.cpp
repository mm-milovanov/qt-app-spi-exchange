#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <app_engine.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    AppEngine appEngine(engine);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.addImportPath(":/module/io-port");
    engine.load(QUrl(u"qrc:/SpiExchangeApp/qml/Main.qml"_qs));

    return app.exec();
}
