#ifndef IDEVICELOOP_H
#define IDEVICELOOP_H

#include <QObject>
#include <QEventLoop>
#include <QTimer>

#include <QDebug>

class IdleWait : public QObject
{
    Q_OBJECT

public:
    IdleWait (int timeout = 0, QObject *parent = nullptr) :
        QObject{parent},
        m_timeout(timeout)
    {
        m_timer.setSingleShot(true);
        QObject::connect(&this->m_timer, &QTimer::timeout,
                         this, &IdleWait::onTimeout);
    }

    ~IdleWait() {
        m_timer.stop();
        m_eventLoop.quit();
        QObject::disconnect(&this->m_timer, &QTimer::timeout,
                         this, &IdleWait::onTimeout);
    }

    void start(void) {
        m_eventLoop.quit();
        m_timer.start(m_timeout);
        m_eventLoop.exec();
    }

    void setTimeout(uint timeout) {
        m_timeout = timeout;
    }

private slots:
    void onTimeout() {
        m_timer.stop();
        m_eventLoop.quit();
    }

private:
    int         m_timeout;
    QTimer      m_timer;
    QEventLoop  m_eventLoop;
};

#endif // IDEVICELOOP_H
