#include "networkobject.h"
#include "Log/logger.h"

#include <QRandomGenerator>
#include <QDateTime>
#include <QCryptographicHash>

NetworkObject::NetworkObject(QObject *parent)
    : QObject{parent}
{
    init();
}

void NetworkObject::init()
{
    quint32 seed = QRandomGenerator::global()->generate();
    QString value = QString("%1-%2").arg(QDateTime::currentDateTime().toString(), QString::number(seed));
    QByteArray key = QCryptographicHash::hash(value.toLocal8Bit(), QCryptographicHash::Md5);
    mBaseToken = key.toHex();
}

QString NetworkObject::getBaseToken() const
{
    return mBaseToken;
}

void NetworkObject::slot_init_network_object()
{
    start();
}

void NetworkObject::slot_start_network_object(const QString &token)
{
    if (token != getBaseToken()) return;
    start();
}

void NetworkObject::slot_stop_network_object(const QString &token, int32_t dwConnID)
{
    LOG_DEBUG("stop network object {} {}", token.toStdString(), dwConnID);
    if (token != getBaseToken()) return;
    stop(dwConnID);
}

void NetworkObject::slot_clear_network_object(const QString &token)
{
    LOG_DEBUG("clear network object {}", token.toStdString());
    if (token != getBaseToken()) return;
    clear();
}

void NetworkObject::slot_delete_network_object(const QString &token)
{
    LOG_DEBUG("close network object {}", token.toStdString());
    if (token != getBaseToken()) return;
    close();
}

void NetworkObject::slot_recreate_network_object(const QString &token, const QString &address, uint16_t port)
{
    LOG_DEBUG("recreate network object {} {} {}", token.toStdString(), address.toStdString(), port);
    if (token != getBaseToken()) return;
    edit(address, port);
}
