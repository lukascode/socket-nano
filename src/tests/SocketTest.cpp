#include "catch.hpp"
#include "../socknano.h"
#include <functional>
#include <iostream>

TEST_CASE("should create tcp socket", "[socket]")
{

    Socket *socket = Socket::CreateSocket(SOCK_STREAM);

    REQUIRE(socket->GetSocket() > 0);
    REQUIRE(socket->GetSocketType() == SOCK_STREAM);

    delete socket;
}

TEST_CASE("should create udp socket", "[socket]")
{

    Socket *socket = Socket::CreateSocket(SOCK_DGRAM);

    REQUIRE(socket->GetSocket() > 0);
    REQUIRE(socket->GetSocketType() == SOCK_DGRAM);

    delete socket;
}

TEST_CASE("should establish tcp connection", "[socket]")
{

    std::thread tcpServer([] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_STREAM);
            servSocket->Bind(new Address(1234));
            servSocket->Listen(20);
            Socket *cliSocket = servSocket->Accept();
            cliSocket->SendAll("HELLO");
            delete servSocket;
        }
        catch (std::exception &e)
        {
            FAIL_CHECK(std::string(e.what()));
        }
    });
    tcpServer.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Socket *socket = Socket::CreateSocket(SOCK_STREAM);
    socket->EnableTimeout(2);
    socket->Connect(new Address(1234));
    auto data = socket->RecvAll(5);
    std::string dataStr = std::string(data.begin(), data.end());

    REQUIRE(data.size() == 5);
    REQUIRE(dataStr == "HELLO");

    delete socket;
}

TEST_CASE("should send and recv datagram", "[socket]")
{

    std::thread udpServer([] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_DGRAM);
            servSocket->Bind(new Address(4321));

            Address *clientAddr;
            auto datagram = servSocket->RecvFrom(clientAddr, 4);
            std::string datagramStr = std::string(datagram.begin(), datagram.end());
            REQUIRE(datagramStr == "PING");

            servSocket->SendTo(clientAddr, "PONG");

            delete servSocket;
        }
        catch (std::exception &e)
        {   
            FAIL_CHECK(std::string(e.what()));
        }

    });
    udpServer.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Address *address = new Address(4321);

    Socket *socket = Socket::CreateSocket(SOCK_DGRAM);
    socket->SendTo(address, "PING");
    
    socket->EnableTimeout(2);

    auto datagram = socket->RecvFrom(address, 4);
    std::string datagramStr = std::string(datagram.begin(), datagram.end());
    REQUIRE(datagramStr == "PONG");

    delete socket;
}