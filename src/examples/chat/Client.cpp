#include "../../socknano.h"
#include <iostream>
#include <thread>

#define PORT 4437

int main(int argc, char** argv)
{
    std::string ip;

    if(argc == 2) {
        ip = std::string(argv[1]);
    }

    std::cout << "Connecting..." << std::endl;

    Socket *socket = Socket::CreateSocket(SOCK_STREAM);
    Address* address = ip.empty() ? new Address(PORT) : new Address(ip, PORT);
    socket->Connect(address);

    std::cout << "Connection established" << std::endl;

    std::string nick;
    std::cout << "Nick: ";
    std::cin >> nick;
    nick += "\n";

    try
    {
        socket->SendAll(nick);
    }
    catch (std::exception &e)
    {
        std::cout<<std::string(e.what())<<std::endl;
        exit(1);
    }

    std::thread sendThread([socket] {
        for (;;)
        {
            std::string msg;
            std::getline(std::cin, msg);
            msg += "\n";
            socket->SendAll(msg);
        }
    });

    std::thread recvThread([socket] {
        for (;;)
        {
            auto data = socket->RecvUntil("\n", 1024);
            std::string msg = std::string(data.begin(), data.end());
            msg = msg.substr(0, msg.size() - 1);
            std::cout << msg << std::endl;
        }
    });

    sendThread.join();
    recvThread.join();
}