#include "../../socknano.h"
#include <iostream>
#include <thread>

#define PORT 4437


int main() 
{

    std::string nick;   
    std::cout<<"Nick: ";
    std::cin>>nick;
    nick += "\n";

    std::cout<<"Connecting..."<<std::endl;

    Socket* socket = Socket::CreateSocket(SOCK_STREAM);
    socket->Connect(new Address(PORT));

    std::cout<<"Connection established"<<std::endl;

    socket->SendAll(nick);

    std::thread sendThread([socket] {
        for(;;)
        {
            std::string msg;
            std::getline(std::cin, msg);
            msg += "\n";
            socket->SendAll(msg);
        }
    });

    std::thread recvThread([socket] {
        for(;;)
        {
            auto data = socket->RecvUntil("\n", 1024);
            std::string msg = std::string(data.begin(), data.end());
            msg = msg.substr(0, msg.size() - 1);
            std::cout<<msg<<std::endl;
        }
    });

    sendThread.join();
    recvThread.join();

}