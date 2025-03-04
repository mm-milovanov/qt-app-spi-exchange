#include "fastboot_client_logs.h"

#include "../fastboot_device.h"
#include "../fastboot_client.h"

FastBootClientLogs::FastBootClientLogs(QObject *parent) :
    QObject{parent},
    m_device(nullptr)
{}

FastBootClientLogs::~FastBootClientLogs()
{
    // if (m_device != nullptr) {
    //     FastBootClient* client = m_device->getClient();
    //     QObject::disconnect(client, &FastBootClient::d, this, &FastBootClientLogs::d);
    //     QObject::disconnect(client, &FastBootClient::i, this, &FastBootClientLogs::i);
    //     QObject::disconnect(client, &FastBootClient::k, this, &FastBootClientLogs::k);
    //     QObject::disconnect(client, &FastBootClient::w, this, &FastBootClientLogs::w);
    //     QObject::disconnect(client, &FastBootClient::e, this, &FastBootClientLogs::e);
    // }
}

void FastBootClientLogs::setDevice(QObject* device) {
    m_device = ((FastBootDevice*)device);
    FastBootClient* client = m_device->getClient();
    QObject::connect(client, &FastBootClient::d, this, &FastBootClientLogs::d);
    QObject::connect(client, &FastBootClient::i, this, &FastBootClientLogs::i);
    QObject::connect(client, &FastBootClient::k, this, &FastBootClientLogs::k);
    QObject::connect(client, &FastBootClient::w, this, &FastBootClientLogs::w);
    QObject::connect(client, &FastBootClient::e, this, &FastBootClientLogs::e);
}

void FastBootClientLogs::clear() {
    m_model.clear();
}

QAbstractListModel* FastBootClientLogs::getModel() {
    return &m_model;
}

void FastBootClientLogs::saveToFile(QUrl url) {
    // TODO: implement
}

void FastBootClientLogs::d(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::DEBUG);
}

void FastBootClientLogs::i(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::INFO);
}

void FastBootClientLogs::k(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::OK);
}

void FastBootClientLogs::w(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::WARNING);
}

void FastBootClientLogs::e(QString tag, QString msg) {
    m_model.append(tag, msg, LogsModel::ERROR);
}

