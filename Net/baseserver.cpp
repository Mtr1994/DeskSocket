#include "baseserver.h"

using namespace mtr;

BaseServer::BaseServer(const QString &ip, uint16_t port)
    : mServerAddress(ip), mServerPort(port)
{

}
