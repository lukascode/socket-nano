#pragma once

#include "Socket.h"
#include "UdpServer.h"

class UdpServer;

class UdpDatagramHandler
{
public:
    virtual ~UdpDatagramHandler();
    virtual void HandleDatagram() = 0;
    void SetSocket(Socket *socket);
    void SetContext(UdpServer *context);
    void SetDatagram(std::string datagram);
    void SetAddress(Address* address);
protected:
    Socket *socket;
    UdpServer *context;
    std::string datagram;
    Address* address;
};