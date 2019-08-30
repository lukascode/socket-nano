#pragma once

#include "UdpDatagramHandler.h"

class UdpDatagramHandler;

class UdpDatagramHandlerFactory
{
public:
    virtual ~UdpDatagramHandlerFactory() {}

    /// Responsible for creating new udp datagram handlers
    virtual UdpDatagramHandler *CreateUdpDatagramHandler() = 0;
};