#include <iostream>
#include "TcpServer.h"
#include "UdpServer.h"
#include "ServerConnectionHandler.h"
#include "ServerConnectionHandlerFactory.h"
#include "NetworkUtils.h"
#include <algorithm>


class TcpConnectionHandler : public ServerConnectionHandler {
public:
	virtual int handleConnection() {
		int err;
		NetworkUtils::print_stdout("New client connected. " + socket->getRemoteAddress(&err).toString() + "\n");
		while(1) {
			std::string data;
			int ret = socket->Recv(&data, 10);
			if(ret == -1) {
				//client close the connection
				NetworkUtils::print_stdout(socket->getRemoteAddress(&err).toString() + ", client close the connection\n");
				context->removeClient(socket);
				pthread_exit(NULL);
			}
			std::string m = socket->getRemoteAddress(&err).toString() + " message: " + data + " l: " + std::to_string(data.length()) + "\n";
			NetworkUtils::print_stdout(m);
		}
	}
};

class TcpConnectionHandlerFactory : public ServerConnectionHandlerFactory {
public:
	virtual ServerConnectionHandler* createServerConnectionHandler() 
	{ return new TcpConnectionHandler(); }
};


int main(int argc, char* argv[]) {

	srand(time(NULL));

	ServerConnectionHandlerFactory* connHandlerFactory = new TcpConnectionHandlerFactory();
	TcpServer* server = new TcpServer(Address(8080), connHandlerFactory);

	int ret = server->Listen();
	if(ret < 0) {
		printf("server listen error\n");
		exit(1);
	}

	
	return 0;
}