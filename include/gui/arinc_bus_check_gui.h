#ifndef CONNECTION_CHECK_PAGE_GUI_H
#define CONNECTION_CHECK_PAGE_GUI_H

#include <QObject>

#include <proto/exchange.h>

#include <gui/board_exchange_logs.h>

class ArincBusCheckPageGUI : public QObject, public ExchangeListener
{
    Q_OBJECT
    Q_PROPERTY(int status READ getStatus NOTIFY statusChanged);
    Q_PROPERTY(uint pkgNum READ getPkgNum NOTIFY pkgNumChanged);
    Q_PROPERTY(int SR READ getSR NOTIFY SRchanged);
    Q_PROPERTY(QString data READ getData NOTIFY dataChanged);

    enum status_e {
        IDLE = 0,
        ACTIVE,
        PAUSE,
        ERROR
    };

    enum alg_status_e {
        MASTER_RESET = 0,
        CHECK_STATUS_REG_0x09,
        WRITE_PKG,
        CHECK_STATUS_REG_0x0E,
        READ_PKG
    };

public:
    explicit ArincBusCheckPageGUI(ExchangeClient* exchange,
                                  QObject *parent = nullptr);

    /* Qml invokable methods */

    Q_INVOKABLE QObject* getLogsCtrl();

    Q_INVOKABLE void start();

    Q_INVOKABLE void stop();

    Q_INVOKABLE void pause();

    /* Qml property methods */

    int getStatus();

    uint getPkgNum();

    int getSR();

    QString getData();

signals:
    void statusChanged(int);

    void pkgNumChanged(uint);

    void SRchanged(int);

    void dataChanged(QString);

protected:
    void onCmdExecuted(QByteArray&) override;

    void onCmdFailed(QByteArray&) override;

private:
    void changeStatus(status_e status);

    void changePkgNum(uint num);

    void changeSR(int SR);

    void algorithmSendStep();

    void algorithmRecvStep(const QByteArray& = QByteArray());

    void writeRequest(uint8_t opCode, QByteArray data);

    void readRequest(uint8_t opCode, int size);

    status_e            m_status;
    alg_status_e        m_algStatus;
    int                 m_index;
    uint                m_n;
    int                 m_SR;
    uint                m_cnt;
    QByteArray          m_pkg;
    BoardExchangeLogs   m_logs;
    ExchangeClient*     m_exchange;
};

#endif // CONNECTION_CHECK_PAGE_GUI_H
