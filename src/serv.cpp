#include "NetworkUtils.h"
#include "TcpServer.h"
#include "ServerConnectionHandler.h"
#include "ServerConnectionHandlerFactory.h"

class TcpConnectionHandler : public ServerConnectionHandler 
{
public:
    virtual int handleConnection() 
    {
        NetworkUtils::print_stdout("New Client: " + socket->getRemoteAddress().toString() + "\n");
        const int size = 4096;
        std::vector<uint8_t> data; 
        int len;
        while(1) {
            int ret = socket->recvall(data, size);
            if(ret < 0) break;
            printf("%s", (char*)data.data());
            FILE* f = fopen("testfile", "w");
            fwrite(data.data(), sizeof(uint8_t), data.size(), f);
            fclose(f);
        }
        NetworkUtils::print_stdout("Client: " + socket->getRemoteAddress().toString() + " disconnected\n");
        context->removeClient(socket);
    }
};

class TcpConnectionHandlerFactory : public ServerConnectionHandlerFactory 
{
public:
    virtual ServerConnectionHandler* createServerConnectionHandler() 
    {
        return new TcpConnectionHandler();
    }
};


int main(int argc, char* argv[]) 
{

    int port = 8090;
    if(argc > 1)
        port = atoi(argv[1]);
    TcpConnectionHandlerFactory* connHandlerFactory = new TcpConnectionHandlerFactory();
    TcpServer* server = new TcpServer(Address(port), connHandlerFactory);
    int ret = server->Listen();
    if(ret < 0) {
        printf("server listen error\n");
        exit(1);
    }

    return 0;
}