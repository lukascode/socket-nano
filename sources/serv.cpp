#include "NetworkUtils.h"
#include "TcpServer.h"
#include "ServerConnectionHandler.h"
#include "ServerConnectionHandlerFactory.h"


class TcpConnectionHandler : public ServerConnectionHandler {
public:
    virtual int handleConnection() {
        NetworkUtils::print_stdout("New Client: " + socket->getRemoteAddress().toString() + "\n");
        uint8_t buf[BSIZE] = {0}; 
        int len;
        const uint8_t* patt = (const uint8_t*)"ABCD";//{65, 66, 67, 68};
        while(1) {
            int ret = socket->recvuntil(buf, 128, patt, 4, &len);
            if(ret < 0) break;
            buf[len] = '\0';
            printf("%s", buf);
        }
        NetworkUtils::print_stdout("Client: " + socket->getRemoteAddress().toString() + " disconnected\n");
        context->removeClient(socket);
    }
};

class TcpConnectionHandlerFactory : public ServerConnectionHandlerFactory {
public:
    virtual ServerConnectionHandler* createServerConnectionHandler() {
        return new TcpConnectionHandler();
    }
};


int main(int argc, char* argv[]) {

    short port = 8090;
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