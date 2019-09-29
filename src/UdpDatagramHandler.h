#pragma once

#include "Socket.h"

class UdpServer;

class UdpDatagramHandler
{
public:
    /// Handles incoming datagram
    virtual void HandleDatagram() = 0;

    /// Sets udp client socket
    void SetSocket(std::shared_ptr<Socket> socket);

    /// Sets UdpServer object as context for handler
    void SetServer(std::shared_ptr<UdpServer> server);

    /// Sets client address
    void SetAddress(std::shared_ptr<Address> address);

    /// Sets datagram as incoming data
    void SetDatagram(std::string datagram);

protected:
    std::shared_ptr<Socket> socket;
    std::shared_ptr<UdpServer> server;
    std::shared_ptr<Address> address;
    std::string datagram;
};