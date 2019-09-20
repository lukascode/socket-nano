#include "../../socknano.h"
#include <iostream>

#define PORT 4437

class ChatHandler : public TcpConnectionHandler
{
public:
    virtual void HandleConnection()
    {
        std::cout << "New connection from: " << socket->GetRemoteAddress().ToString() << std::endl;
        std::string msg;
        std::string nick;
        try
        {
            // user sets nickname
            {
                socket->EnableTimeout(15);
                auto data = socket->RecvUntil("\n", 32);
                nick = std::string(data.begin(), data.end());
                nick = nick.substr(0, nick.size() - 1);
                msg = nick + " joined room\n";
                server->Broadcast(msg, socket);
                socket->DisableTimeout();
            }

            // listen for incoming messages and broadcast
            while (server->IsListening())
            {
                auto message = socket->RecvUntil("\n", 1024);
                std::string msgStr = std::string(message.begin(), message.end());
                if (msgStr.length() > 1)
                {
                    msg = nick + ": " + msgStr;
                    server->Broadcast(msg, socket);
                }
            }
        }
        catch (std::exception &e)
        {
            std::cerr << std::string(e.what()) << std::endl;
        }
        if (!nick.empty())
        {
            msg = nick + " left room\n";
            server->Broadcast(msg);
        }
    }
};

int main()
{

    TcpServer server([] { return new ChatHandler(); });

    server.Listen(PORT);

    return 0;
}