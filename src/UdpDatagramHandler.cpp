#include "UdpDatagramHandler.h"

UdpDatagramHandler::~UdpDatagramHandler()
{
    if (address)
        delete address;
}

void UdpDatagramHandler::setSocket(Socket *socket)
{
    this->socket = socket;
}

void UdpDatagramHandler::setContext(UdpServer *context)
{
    this->context = context;
}

void UdpDatagramHandler::setDatagram(std::string datagram)
{
    this->datagram = datagram;
}

void UdpDatagramHandler::setAddress(Address* address)
{
    this->address = address;
}