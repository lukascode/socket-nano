#pragma once

#include "UdpDatagramHandler.h"

class UdpDatagramHandler;

class UdpDatagramHandlerFactory
{
public:
    virtual ~UdpDatagramHandlerFactory() {}
    virtual UdpDatagramHandler *CreateUdpDatagramHandler() = 0;
};