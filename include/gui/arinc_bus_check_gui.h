#ifndef CONNECTION_CHECK_PAGE_GUI_H
#define CONNECTION_CHECK_PAGE_GUI_H

#include <QObject>
#include <proto/exchange.h>

class ConnectionCheckPageGUI : public QObject, public ExchangeListener
{
    Q_OBJECT
public:
    explicit ConnectionCheckPageGUI(ExchangeClient* exchange,
                                    QObject *parent = nullptr);

signals:

protected:
    void onCmdExecuted(QByteArray&) override;
    void onCmdFailed(QByteArray&) override;

private:
    void writeRequest(uint8_t opCode, QByteArray data);
    void readRequest(uint8_t opCode, int size);

    ExchangeClient* m_exchange;
};

#endif // CONNECTION_CHECK_PAGE_GUI_H
