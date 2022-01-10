#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <QString>
#include <QByteArray>

enum Protocol{ TCP = 1, UDP, BROADCAST };

enum {Tcp_Server, Tcp_Client_Slave, Tcp_Client, Udp_Server, Udp_Client};

enum ServerStatus{Server_Started = 1, Server_Shutdown, Server_Starting};

enum ClientStatus{Client_Connected = 1, Client_Closed, Client_Connecting};

typedef struct {
    int type; // 区分被动接受的套接字和主动连接的套接字
    QString serverkey;
    QString socketkey;
    qintptr socketDescriptor;
    QString error;
    QByteArray data;
}ClientInfo;

typedef struct {
    QString key;
    QString name;
    QString ip;
    uint32_t port;
}ServerInfo;

#endif // DEFINES_H
