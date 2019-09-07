#include "catch.hpp"
#include "../socknano.h"
#include <functional>
#include "TestUtils.h"

TEST_CASE("should create tcp socket", "[socket]")
{

    Socket *socket = Socket::CreateSocket(SOCK_STREAM);

    REQUIRE(socket->GetSocket() > 0);
    REQUIRE(socket->Valid());
    REQUIRE(socket->GetSocketType() == SOCK_STREAM);

    delete socket;
}

TEST_CASE("should create udp socket", "[socket]")
{

    Socket *socket = Socket::CreateSocket(SOCK_DGRAM);

    REQUIRE(socket->GetSocket() > 0);
    REQUIRE(socket->Valid());
    REQUIRE(socket->GetSocketType() == SOCK_DGRAM);

    delete socket;
}

TEST_CASE("should throw when provided fd by constructor is broken", "[socket]")
{
    try
    {
        Socket socket(-100);
        FAIL_CHECK("Expected SocketException");
    }
    catch (SocketException &e)
    {
        REQUIRE(std::string(e.what()) == "Invalid socket descriptor");
    }
}

TEST_CASE("should create tcp socket based on another one", "[socket]")
{
    Socket *s1 = Socket::CreateSocket(SOCK_STREAM);
    Socket *s2 = new Socket(s1->GetSocket());

    REQUIRE(s2->GetSocketType() == SOCK_STREAM);
    REQUIRE(s2->Valid());

    delete s1;
    delete s2;
}

TEST_CASE("should create udp socket based on another one", "[socket]")
{
    Socket *s1 = Socket::CreateSocket(SOCK_DGRAM);
    Socket *s2 = new Socket(s1->GetSocket());

    REQUIRE(s2->GetSocketType() == SOCK_DGRAM);
    REQUIRE(s2->Valid());

    delete s1;
    delete s2;
}

TEST_CASE("should establish tcp connection", "[socket]")
{
    uint16_t port = RandomPort();
    std::thread tcpServer([port] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_STREAM);
            servSocket->Bind(new Address(port));
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
    socket->Connect(new Address(port));
    auto data = socket->RecvAll(5);
    std::string dataStr = std::string(data.begin(), data.end());

    REQUIRE(data.size() == 5);
    REQUIRE(dataStr == "HELLO");

    delete socket;
}

TEST_CASE("should throw timeout exception", "[socket]")
{
    uint16_t port = RandomPort();
    std::thread tcpServer([port] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_STREAM);
            servSocket->Bind(new Address(port));
            servSocket->Listen(20);
            servSocket->Accept();
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
        catch (std::exception &e)
        {
            FAIL_CHECK(std::string(e.what()));
        }
    });
    tcpServer.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    try
    {
        Socket *socket = Socket::CreateSocket(SOCK_STREAM);
        socket->EnableTimeout(5);
        socket->Connect(new Address(port));
        socket->RecvAll(32);
        FAIL_CHECK("Expected TimeoutException");
    }
    catch (TimeoutException &e)
    {
        REQUIRE(std::string(e.what()) == "Waiting time has been exceeded");
    }
}

TEST_CASE("recv until test", "[socket]")
{
    uint16_t port = RandomPort();
    std::thread tcpServer([port] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_STREAM);
            servSocket->Bind(new Address(port));
            servSocket->Listen(20);
            Socket *socket = servSocket->Accept();
            socket->SendAll("ggw9gijskgjb0943AA\r\n5gbnlklsjfb");
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
    socket->Connect(new Address(port));
    auto data = socket->RecvUntil("AA\r\n", 128);

    REQUIRE(std::string(data.begin(), data.end()) == "ggw9gijskgjb0943AA\r\n");
}

TEST_CASE("recv until test expect buffer overflow", "[socket]")
{
    uint16_t port = RandomPort();
    std::thread tcpServer([port] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_STREAM);
            servSocket->Bind(new Address(port));
            servSocket->Listen(20);
            Socket *socket = servSocket->Accept();
            socket->SendAll("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n\r\n");
        }
        catch (std::exception &e)
        {
            FAIL_CHECK(std::string(e.what()));
        }
    });
    tcpServer.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    try
    {
        Socket *socket = Socket::CreateSocket(SOCK_STREAM);
        socket->Connect(new Address(port));
        auto data = socket->RecvUntil("\r\n\r\n", 16);
        FAIL_CHECK("Expected std::overflow_error");
    }
    catch (std::overflow_error &e)
    {
        REQUIRE(std::string(e.what()) == "recvuntil error: Overflow error");
    }
}

TEST_CASE("should send and recv datagram", "[socket]")
{
    uint16_t port = RandomPort();
    std::thread udpServer([port] {
        try
        {
            Socket *servSocket = Socket::CreateSocket(SOCK_DGRAM);
            servSocket->Bind(new Address(port));

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

    Address *address = new Address(port);

    Socket *socket = Socket::CreateSocket(SOCK_DGRAM);
    socket->SendTo(address, "PING");

    socket->EnableTimeout(2);

    auto datagram = socket->RecvFrom(address, 4);
    std::string datagramStr = std::string(datagram.begin(), datagram.end());
    REQUIRE(datagramStr == "PONG");

    delete socket;
}