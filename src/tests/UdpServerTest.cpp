#include "catch.hpp"
#include "../socknano.h"
#include <functional>
#include <atomic>
#include "TestUtils.h"

TEST_CASE("udp server general test", "[udp-server]")
{
    class Handler : public UdpDatagramHandler
    {
    public:
        std::atomic<bool> &handlerStarted;
        Handler(std::atomic<bool> &handlerStarted) : handlerStarted(handlerStarted) {}
        virtual void HandleDatagram() { handlerStarted = true; }
    };

    std::atomic<bool> handlerStarted(false);

    short port = RandomPort();
    UdpServer* server = new UdpServer([&handlerStarted] { return new Handler(handlerStarted); });

    std::thread serverThread([server, port] {
        server->Listen(port);
    });
    serverThread.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    try {
        server->Listen(port);
        FAIL_CHECK("Expected UdpServerException");
    } catch(UdpServerException& e) {
        REQUIRE(std::string(e.what()) == "Already listening");
    }

    Socket* socket = Socket::CreateSocket(SOCK_DGRAM);
    socket->SendTo(new Address(port), "Test");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    REQUIRE(server->IsListening());
    REQUIRE(handlerStarted.load());

    server->Stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    REQUIRE(!server->IsListening());
}

TEST_CASE("should transfer datagram properly", "[udp-server]")
{
    std::string DATAGRAM = "945itgjoiofss0-9f-w";

    class Handler : public UdpDatagramHandler
    {
    public:
        std::string& receivedDatagram;
        Handler(std::string& receivedDatagram) : receivedDatagram(receivedDatagram) {}
        virtual void HandleDatagram() {
            receivedDatagram = datagram;
            socket->SendTo(address, datagram);
        }
    };   

    std::string receivedDatagramByServer;
    short port = RandomPort();
    UdpServer* server = new UdpServer([&receivedDatagramByServer] { return new Handler(receivedDatagramByServer); });

    std::thread serverThread([server, port] {
        server->Listen(port);
    });
    serverThread.detach();

    // wait for server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    REQUIRE(server->IsListening());

    Socket* socket = Socket::CreateSocket(SOCK_DGRAM);

    Address* serverAddr = new Address(port);

    socket->SendTo(serverAddr, DATAGRAM);
    auto data = socket->RecvFrom(serverAddr, DATAGRAM.size());
    std::string receivedDatagramByClient = std::string(data.begin(), data.end());


    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    REQUIRE(receivedDatagramByServer == DATAGRAM);
    REQUIRE(receivedDatagramByClient == DATAGRAM);

    server->Stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    REQUIRE(!server->IsListening());
}