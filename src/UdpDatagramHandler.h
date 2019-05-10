#pragma once

#include "Socket.h"
#include "UdpServer.h"

class UdpServer;

class UdpDatagramHandler
{
public:
    virtual ~UdpDatagramHandler();
    virtual void handleDatagram() = 0;
    void setSocket(Socket *socket);
    void setContext(UdpServer *context);
    void setDatagram(std::string datagram);
    void setAddress(Address* address);
protected:
    Socket *socket;
    UdpServer *context;
    std::string datagram;
    Address* address;
};