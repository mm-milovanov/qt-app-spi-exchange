#include <gui/board_exchange_logs.h>

BoardExchangeLogs::BoardExchangeLogs(QObject *parent) :
    QObject{parent}
{}

BoardExchangeLogs::~BoardExchangeLogs()
{ }

void BoardExchangeLogs::clear() {
    m_model.clear();
}

QAbstractListModel* BoardExchangeLogs::getModel() {
    return &m_model;
}

void BoardExchangeLogs::d(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::DEBUG);
}

void BoardExchangeLogs::i(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::INFO);
}

void BoardExchangeLogs::k(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::OK);
}

void BoardExchangeLogs::w(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::WARNING);
}

void BoardExchangeLogs::e(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::ERROR);
}

