#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <QString>
#include <QByteArray>

enum Protocol { TCP = 1, UDP, BROADCAST };

enum {Tcp_Server, Udp_Server, Tcp_Client_Slave, Udp_Client_Slave, Tcp_Client, Udp_Client, Udp_Client_Cast};

enum ServerStatus{Server_Starting = 0, Server_Started , Server_Stoping, Server_Stoped};

enum ClientStatus{Client_Connecting = 0, Client_Connected, Client_Stoping, Client_Stoped};

typedef struct
{
    int type;
    int status;
    QString token;
    QString name;
    QString peerAddress;
    uint16_t peerPort;
    QString localAddress;
    uint16_t localPort;
    uint16_t clientNumber;
}NetworkObjectDetail;

#endif // DEFINES_H
