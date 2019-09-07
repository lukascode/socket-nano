#pragma once

#include "Socket.h"

class UdpServer;

class UdpDatagramHandler
{
public:
    virtual ~UdpDatagramHandler();

    /// Handles incoming datagram 
    virtual void HandleDatagram() = 0;

    /// Sets udp client socket
    void SetSocket(Socket *socket);

    /// Sets UdpServer object as context for handler
    void SetServer(UdpServer *server);

    /// Sets datagram as incoming data
    void SetDatagram(std::string datagram);

    /// Sets client address
    void SetAddress(Address* address);
protected:
    Socket *socket;
    UdpServer *server;
    std::string datagram;
    Address* address;
};