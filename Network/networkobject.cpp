#include "networkobject.h"

#include <QRandomGenerator>
#include <QDateTime>
#include <QCryptographicHash>

// test
#include <QDebug>

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
    qDebug() << "NetworkObject::slot_stop_network_object " << token << getBaseToken() << dwConnID;
    if (token != getBaseToken()) return;
    stop(dwConnID);
}

void NetworkObject::slot_clear_network_object(const QString &token)
{
    if (token != getBaseToken()) return;
    clear();
}

void NetworkObject::slot_delete_network_object(const QString &token)
{
    if (token != getBaseToken()) return;
    close();
}
