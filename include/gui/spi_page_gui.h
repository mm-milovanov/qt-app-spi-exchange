#ifndef SPI_PAGE_GUI_H
#define SPI_PAGE_GUI_H

#include <QObject>
#include <proto/exchange.h>

#include <unordered_map>

class ComboboxGUI;

class SpiPageGUI : public QObject, public ExchangeListener
{
    Q_OBJECT

public:
    explicit SpiPageGUI(ExchangeClient* exchange,
                        QObject *parent = nullptr);
    ~SpiPageGUI();

    Q_INVOKABLE void writeRequest(uint8_t opCode, QString data);
    Q_INVOKABLE void readRequest(uint8_t opCode, int size);

    Q_INVOKABLE int getDataSize(uint8_t opCode);
    Q_INVOKABLE int getRwPermision(uint8_t opCode);

signals:
    void selectorChanged();
    Q_SIGNAL void receivedData(QString);

protected:
    void onCmdExecuted(QByteArray&) override;
    void onCmdFailed(QByteArray&) override;

private:
    std::unordered_map <int, std::pair<int, int>> m_regsConf;
    ExchangeClient* m_exchange;
};

#endif // SPI_PAGE_GUI_H
